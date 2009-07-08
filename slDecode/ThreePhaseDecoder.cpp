#include "ThreePhaseDecoder.h"

#define maximum(a, b, c) \
	(a > b && a > c ? a : \
	(b > a && b > c ? b : c))

#define minimum(a, b, c) \
	(a < b && a < c ? a : \
	(b < a && b < c ? b : c))

void ThreePhaseDecoder::setup(int width, int height, float noiseTolerance) {
	this->width = width;
	this->height = height;
	this->noiseTolerance = noiseTolerance;

	phase1Image.allocate(width, height, OF_IMAGE_COLOR);
	phase2Image.allocate(width, height, OF_IMAGE_COLOR);
	phase3Image.allocate(width, height, OF_IMAGE_COLOR);

	_wrapphase = new float*[height];
	_mask = new bool*[height];
	_process = new bool*[height];
	for(int i = 0; i < height; i++) {
		_wrapphase[i] = new float[width];
		_mask[i] = new bool[width];
		_process[i] = new bool[width];
		for(int j = 0; j < width; j++) {
			_wrapphase[i][j] = 0;
			_mask[i][j] = false;
			_process[i][j] = false;
		}
	}
}

int ThreePhaseDecoder::getWidth() {
	return width;
}

int ThreePhaseDecoder::getHeight() {
	return height;
}

bool** ThreePhaseDecoder::getProcess() {
	return _process;
}

float** ThreePhaseDecoder::getWrapPhase() {
	return _wrapphase;
}

void ThreePhaseDecoder::decode() {
	loadImages();
  phaseUnwrapAll();
  propagatePhases();
}

void ThreePhaseDecoder::loadImages() {
  phase1Image.loadImage("phase1.jpg");
  phase2Image.loadImage("phase2.jpg");
  phase3Image.loadImage("phase3.jpg");

  phase1Image.setImageType(OF_IMAGE_GRAYSCALE);
  phase2Image.setImageType(OF_IMAGE_GRAYSCALE);
  phase3Image.setImageType(OF_IMAGE_GRAYSCALE);
}

void ThreePhaseDecoder::phaseUnwrapAll() {
	unsigned char* phase1Pixels = phase1Image.getPixels();
	unsigned char* phase2Pixels = phase2Image.getPixels();
	unsigned char* phase3Pixels = phase3Image.getPixels();
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int i = x + y * width;

      float phase1 = (phase1Pixels[i] & 255) / 255.;
      float phase2 = (phase2Pixels[i] & 255) / 255.;
      float phase3 = (phase3Pixels[i] & 255) / 255.;

      float phaseSum = phase1 + phase2 + phase3;
      float phaseRange = maximum(phase1, phase2, phase3) - minimum(phase1, phase2, phase3);

      // avoid the noise floor
      float gamma = phaseRange / phaseSum;
      _mask[y][x] = gamma < noiseTolerance;
      _process[y][x] = true;

      float theta = phaseUnwrap(phase1, phase2, phase3);
      _wrapphase[y][x] = theta;
    }
  }
}

float ThreePhaseDecoder::phaseUnwrap(float phase1, float phase2, float phase3) {
  bool flip;
  int off;
  float maxPhase, medPhase, minPhase;
  if(phase2 <= phase3 && phase3 <= phase1) {
    flip = false;
    off = 0;
    maxPhase = phase1;
    medPhase = phase3;
    minPhase = phase2;
  } else if(phase2 <= phase1 && phase1 <= phase3) {
    flip = true;
    off = 2;
    maxPhase = phase3;
    medPhase = phase1;
    minPhase = phase2;
  } else if(phase1 <= phase2 && phase2 <= phase3) {
    flip = false;
    off = 2;
    maxPhase = phase3;
    medPhase = phase2;
    minPhase = phase1;
  } else if(phase1 <= phase3 && phase3 <= phase2) {
    flip = true;
    off = 4;
    maxPhase = phase2;
    medPhase = phase3;
    minPhase = phase1;
  } else if(phase3 <= phase1 && phase1 <= phase2) {
    flip = false;
    off = 4;
    maxPhase = phase2;
    medPhase = phase1;
    minPhase = phase3;
  } else {
    flip = true;
    off = 6;
    maxPhase = phase1;
    medPhase = phase2;
    minPhase = phase3;
  }
  float theta = 0;
  if(maxPhase != minPhase)
    theta = (medPhase-minPhase) / (maxPhase-minPhase);
  if (flip)
    theta = -theta;
  theta += off;
  return theta / 6;
}

void ThreePhaseDecoder::propagatePhases() {
  int startX = width / 2;
  int startY = height / 2;

  toProcess.clear();
  int2 start = {startX, startY};
  toProcess.push_back(start);
  _process[startX][startY] = false;

  while (!toProcess.empty()) {
    int2 xy = toProcess.front();
    toProcess.pop_front();
    int x = xy.x;
    int y = xy.y;
    float r = _wrapphase[y][x];

    // propagate in each direction, so long as
    // it isn't masked and it hasn't already been processed
    if (y > 0 && !_mask[y-1][x] && _process[y-1][x])
      unwrap(r, x, y-1);
    if (y < height-1 && !_mask[y+1][x] && _process[y+1][x])
      unwrap(r, x, y+1);
    if (x > 0 && !_mask[y][x-1] && _process[y][x-1])
      unwrap(r, x-1, y);
    if (x < width-1 && !_mask[y][x+1] && _process[y][x+1])
      unwrap(r, x+1, y);
  }
}

void ThreePhaseDecoder::unwrap(float r, int x, int y) {
  float frac = r - floor(r);
  float myr = _wrapphase[y][x] - frac;
  if (myr > .5)
    myr--;
  if (myr < -.5)
    myr++;

  _wrapphase[y][x] = myr + r;
  _process[y][x] = false;
  int2 cur = {x, y};
  toProcess.push_back(cur);
}

ThreePhaseDecoder::~ThreePhaseDecoder() {
	for(int i = 0; i < height; i++)
		delete [] _wrapphase[i], _mask[i], _process[i];
	delete [] _wrapphase, _mask, _process;
}
