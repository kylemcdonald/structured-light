#include "ThreePhaseDecoder.h"

#define maximum(a, b, c) \
	(a > b && a > c ? a : \
	(b > a && b > c ? b : c))

#define minimum(a, b, c) \
	(a < b && a < c ? a : \
	(b < a && b < c ? b : c))

void ThreePhaseDecoder::setup(int width, int height, float zskew, float zscale, float noiseTolerance) {
	this->width = width;
	this->height = height;
	this->zskew = zskew;
	this->zscale = zscale;
	this->noiseTolerance = noiseTolerance;

	toProcess.reserve(height * width);

	wrapphase = new float*[height];
	mask = new bool*[height];
	process = new bool*[height];
	depth = new float*[height];
	color = new unsigned char*[height];
	for(int i = 0; i < height; i++) {
		wrapphase[i] = new float[width];
		mask[i] = new bool[width];
		process[i] = new bool[width];
		depth[i] = new float[width];
		color[i] = new unsigned char[width * 3];
	}

	for(int phase = 0; phase < 3; phase++) {
		imgColor[phase].allocate(width, height, OF_IMAGE_COLOR);
		img[phase].allocate(width, height, OF_IMAGE_GRAYSCALE);
	}

	wide = false;
}

int ThreePhaseDecoder::getWidth() {
	return width;
}

int ThreePhaseDecoder::getHeight() {
	return height;
}

bool** ThreePhaseDecoder::getMask() {
	return mask;
}

float** ThreePhaseDecoder::getDepth() {
	return depth;
}

unsigned char** ThreePhaseDecoder::getColor() {
	return color;
}

void ThreePhaseDecoder::loadImages(
	string phase1Filename,
	string phase2Filename,
	string phase3Filename) {

	loadImage(0, phase1Filename);
	loadImage(1, phase2Filename);
	loadImage(2, phase3Filename);
}

void ThreePhaseDecoder::loadImage(int phase, string filename) {
  imgColor[phase].loadImage(filename);
	img[phase] = imgColor[phase];
  img[phase].setImageType(OF_IMAGE_GRAYSCALE);
}

void ThreePhaseDecoder::setImage(int phase, ofImage& img) {
	imgColor[phase] = img;
	this->img[phase] = imgColor[phase];
	this->img[phase].setImageType(OF_IMAGE_GRAYSCALE);
}

void ThreePhaseDecoder::updateColor() {
	unsigned char* phase1Pixels = imgColor[0].getPixels();
	unsigned char* phase2Pixels = imgColor[1].getPixels();
	unsigned char* phase3Pixels = imgColor[2].getPixels();

	for(int y = 0; y < height; y++) {
  	int offset = y * width * 3;
  	for(int xc = 0; xc < width * 3; xc++) {
  		color[y][xc] = maximum(
				phase1Pixels[offset],
				phase2Pixels[offset],
				phase3Pixels[offset]);
  		offset++;
  	}
  }
}

void ThreePhaseDecoder::decode() {
	updateColor();
	phaseWrap();
	phaseUnwrap();
	makeDepth();
}

void ThreePhaseDecoder::phaseWrap() {
	unsigned char* phase1Pixels = img[0].getPixels();
	unsigned char* phase2Pixels = img[1].getPixels();
	unsigned char* phase3Pixels = img[2].getPixels();
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int i = x + y * width;

      float phaseSum = phase1Pixels[i] + phase2Pixels[i] + phase3Pixels[i];
      float phaseRange =
				maximum(phase1Pixels[i], phase2Pixels[i], phase3Pixels[i]) -
				minimum(phase1Pixels[i], phase2Pixels[i], phase3Pixels[i]);

      if(phaseRange / phaseSum < noiseTolerance) {
				mask[y][x] = true;
				process[y][x] = false;
			} else {
				mask[y][x] = false;
				process[y][x] = true;
				wrapphase[y][x] = phaseWrap(phase1Pixels[i], phase2Pixels[i],	phase3Pixels[i]);
			}
    }
  }
}

float ThreePhaseDecoder::phaseWrap(
		const unsigned char& phase1,
		const unsigned char& phase2,
		const unsigned char& phase3) {
  bool flip;
  int off;
  unsigned char maxPhase, medPhase, minPhase;
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
    theta = (float) (medPhase-minPhase) / (maxPhase-minPhase);
  if (flip)
    theta = -theta;
	theta += off;
  return theta / 6;
}

void ThreePhaseDecoder::phaseUnwrap() {
  int startX = width / 2;
  int startY = height / 2;

  for(int i = 0; i < width / 2; i++) {
  	if(process[startY][startX + i]) {
  		startX += i;
			break;
  	}
		if(process[startY][startX - i]) {
			startX -= i;
			break;
		}
  }

  toProcess.clear();
  intPoint start = {startX, startY};
  toProcess.push_back(start);
  process[startY][startX] = false;

  while (!toProcess.empty()) {
    intPoint xy = toProcess.front();
    toProcess.pop_front();
    int& x = xy.x;
    int& y = xy.y;
    float& r = wrapphase[y][x];

    // propagate in each direction, so long as
    // it hasn't already been processed
    if (y > 0 && process[y-1][x])
      phaseUnwrap(r, x, y-1);
    if (y < height-1 && process[y+1][x])
      phaseUnwrap(r, x, y+1);
    if (x > 0 && process[y][x-1])
      phaseUnwrap(r, x-1, y);
    if (x < width-1 && process[y][x+1])
      phaseUnwrap(r, x+1, y);
  }
}

void ThreePhaseDecoder::phaseUnwrap(const float& r, int x, int y) {
	float frac = r - floorf(r);
  float myr = wrapphase[y][x] - frac;
  if (myr > .5)
    myr--;
  else if (myr < -.5)
    myr++;

  wrapphase[y][x] = myr + r;
  process[y][x] = false;
  intPoint cur = {x, y};
  toProcess.push_back(cur);
}

void ThreePhaseDecoder::makeDepth() {
	if(wide) {
		for (int y = 0; y < height; y ++) {
			for (int x = 0; x < width; x++) {
				float planephase = 0.5 - (x - (width / 2)) / -zskew;
				if (!mask[y][x])
					depth[y][x] = (wrapphase[y][x] - planephase) * zscale;
			}
		}
	} else {
		for (int y = 0; y < height; y ++) {
			float planephase = 0.5 - (y - (height / 2)) / zskew;
			for (int x = 0; x < width; x++)
				if (!mask[y][x])
					depth[y][x] = (wrapphase[y][x] - planephase) * zscale;
		}
	}
}

ThreePhaseDecoder::~ThreePhaseDecoder() {
	for(int i = 0; i < height; i++)
		delete [] wrapphase[i], mask[i], process[i], depth[i], color[i];
	delete [] wrapphase, mask, process, depth, color;
}
