#include "PhaseDecoder.h"

PhaseDecoder::PhaseDecoder() :
	sequenceSize(0),
	gamma(1.0),
	colorSequence(NULL),
	graySequence(NULL),
	phase(NULL),
	ready(NULL),
	depthScale(1),
	depthSkew(1),
	maxPasses(1),
	minRemaining(0),
	color(NULL),
	orientation(PHASE_VERTICAL),
	phasePersistence(false),
	lastPhase(false) {
}

// This code should migrate to DepthDecoder, as it will
// be used for allocating a gray coded image sequence also.
void PhaseDecoder::setup(int width, int height, int sequenceSize, int numColorChan) {
	this->width = width;
	this->height = height;
	this->sequenceSize = sequenceSize;
	this->numColorChan = numColorChan;

	colorSequence = new byte*[sequenceSize];
	graySequence = new byte*[sequenceSize];
	int n = width * height;
	for(int i = 0; i < sequenceSize; i++) {
		colorSequence[i] = new byte[n * 3];
		graySequence[i] = new byte[n];
	}
	phase = new float[n];
	wrappedPhase = new float[n];
	mask = new bool[n];
	ready = new bool[n];
	depth = new float[n];
	color = new byte[n * 3];

	blur.setup(width, height);

	lastPhase = new float[n];
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
		delete [] ready;
		delete [] color;
		delete [] lastPhase;
	}
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

    if (numColorChan == 3) {
        memcpy(curColor, image, width * height * 3);
    }

	int n = width * height;
	int i = 0;
	int j = 0;
	while(j < n) {
	    if (numColorChan == 1) {
            curColor[i]   = image[j];
            curColor[i+1] = image[j];
            curColor[i+2] = image[j];
	    }

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
	memcpy( wrappedPhase,phase, sizeof(float) * width * height);
	for(int pass = 0; pass < maxPasses; pass++) {
		unwrapPhase();
		if(minRemaining != 0 && getRemaining() < minRemaining)
			break;
	}
	if(phasePersistence)
		memcpy(lastPhase, phase, sizeof(float) * width * height);
	makeColor();
	makeDepth();
}

float* PhaseDecoder::getPhase() {
	return phase;
}

float* PhaseDecoder::getWrappedPhase() {
    return wrappedPhase;
}

void PhaseDecoder::setGamma(float gamma) {
    this->gamma = gamma;
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

void PhaseDecoder::clearLastPhase() {
	memset(lastPhase, 0, sizeof(float) * width * height);
}

void PhaseDecoder::makeDepth() {
    maxDepth = -1e6;
    minDepth = 1e6;
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

			if (depth[i] > maxDepth) maxDepth = depth[i];
			if (depth[i] < minDepth) minDepth = depth[i];
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

			if (depth[i] > maxDepth) maxDepth = depth[i];
			if (depth[i] < minDepth) minDepth = depth[i];
		}
	}
}

byte* PhaseDecoder::getColor() {
	return color;
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

void PhaseDecoder::exportCloud(string filename) {
	DepthExporter::exportCloud(filename, width, height, mask, depth, color);
}

void PhaseDecoder::exportMesh(string filename) {
	DepthExporter::exportMesh(filename, width, height, mask, depth, color);
}
