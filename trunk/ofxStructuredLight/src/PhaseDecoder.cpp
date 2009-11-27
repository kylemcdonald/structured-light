#include "PhaseDecoder.h"

PhaseDecoder::PhaseDecoder() {
	width = 0;
	height = 0;
	sequenceSize = 0;
	colorSequence = NULL;
	graySequence = NULL;
	phase = NULL;
	mask = NULL;
	ready = NULL;
	depth = NULL;
	depthScale = 1;
	depthSkew = 1;
	maxPasses = 1;
	minRemaining = 0;
	color = NULL;
	orientation = PHASE_VERTICAL;
	phasePersistence = false;
	lastPhase = false;
}

void PhaseDecoder::setup(int width, int height, int sequenceSize) {
	this->width = width;
	this->height = height;
	this->sequenceSize = sequenceSize;

	colorSequence = new byte*[sequenceSize];
	graySequence = new byte*[sequenceSize];
	int n = width * height;
	for(int i = 0; i < sequenceSize; i++) {
		colorSequence[i] = new byte[n * 3];
		graySequence[i] = new byte[n];
	}
	phase = new float[n];
	mask = new bool[n];
	ready = new bool[n];
	depth = new float[n];
	color = new byte[n * 3];

	blur.setup(width, height);

	lastPhase = new float[n];
}

int PhaseDecoder::getWidth() {
	return width;
}

int PhaseDecoder::getHeight() {
	return height;
}

void PhaseDecoder::setMaxPasses(int maxPasses) {
	this->maxPasses = maxPasses;
}

void PhaseDecoder::setMinRemaining(float minRemaining) {
	this->minRemaining = minRemaining;
}

void PhaseDecoder::set(int position, byte* image) {
	byte* curColor = colorSequence[position];
	byte* curGray = graySequence[position];
	memcpy(curColor, image, width * height * 3);
	int n = width * height;
	int i = 0;
	int j = 0;
	while(j < n) {
		float sum =
			(float) curColor[i++] +
			(float) curColor[i++] +
			(float) curColor[i++];
		sum /= 3;
		curGray[j++] = (byte) sum;
	}
}

void PhaseDecoder::decode() {
	makePhase();
	int pass;
	for(pass = 0; pass < maxPasses; pass++) {
		unwrapPhase();
		if(getRemaining() < minRemaining)
			break;
	}
	if(phasePersistence)
		memcpy(lastPhase, phase, sizeof(float) * width * height);
	makeDepth();
	makeColor();
}

float* PhaseDecoder::getPhase() {
	return phase;
}

bool* PhaseDecoder::getMask() {
	return mask;
}

void PhaseDecoder::setDepthScale(float depthScale) {
	this->depthScale = depthScale;
}

void PhaseDecoder::setDepthSkew(float depthSkew) {
	this->depthSkew = depthSkew;
}

void PhaseDecoder::setOrientation(phaseOrientation orientation) {
	this->orientation = orientation;
}

void PhaseDecoder::setPhasePersistence(bool phasePersistence) {
	this->phasePersistence = phasePersistence;
}

void PhaseDecoder::makeDepth() {
	int n = width * height;
	if(orientation == PHASE_VERTICAL) {
		for (int i = 0; i < n; i++) {
			if (!mask[i]) {
				float x = (float) (i % width);
				float planephase = ((x / width) - .5f) * depthSkew;
				depth[i] = (phase[i] - planephase) * depthScale;
			} else {
				depth[i] = 0;
			}
		}
	} else if(orientation == PHASE_HORIZONTAL) {
			for (int i = 0; i < n; i++) {
				if (!mask[i]) {
					float y = (float) (i / width);
					float planephase = ((y / height) - .5f) * depthSkew;
					depth[i] = (phase[i] - planephase) * depthScale;
				} else {
					depth[i] = 0;
			}
		}
	}
}

float* PhaseDecoder::getDepth() {
	return depth;
}

byte* PhaseDecoder::getColor() {
	return color;
}

void PhaseDecoder::save(string filename) {
	ofstream out;
	out.open(filename.c_str());
	int n = width * height;
	for(int i = 0; i < n; i++) {
		if(!mask[i]) {
			int x = i % width;
			int y = i / width;
			float z = depth[i];
			out << "v\t" << x << "\t" << y << "\t" << z << "\n";
		}
	}
	out <<	"p ";
	int j = 1;
	for(int i = 0; i < n; i++)
		if(!mask[i])
			out << "v" << (j++) << " ";
	out << "\n";
	out.close();
}

float PhaseDecoder::getRemaining() {
	int n = width * height;
	int readySum = 0;
	int maskSum = 0;
	for(int i = 0; i < n; i++) {
		readySum += (unsigned char) ready[i];
		maskSum += (unsigned char) mask[i];
	}
	return (float) readySum / (float) (n - maskSum);
}

int PhaseDecoder::getStart() {
	blur.blur((unsigned char*) ready, ready, 64);
	int* sum = blur.getSum();
	int max = 0;
	int n = width * height;
	for(int i = 0; i < n; i++)
		if(sum[i] > sum[max])
			max = i;
	return max;
}

int* PhaseDecoder::getBlur() {
	return blur.getSum();
}

PhaseDecoder::~PhaseDecoder() {
	if(colorSequence != NULL) {
		for(int i = 0; i < sequenceSize; i++) {
				delete [] colorSequence[i];
				delete [] graySequence[i];
		}
		delete [] colorSequence;
		delete [] graySequence;
		delete [] phase;
		delete [] mask;
		delete [] ready;
		delete [] depth;
		delete [] color;
		delete [] lastPhase;
	}
}
