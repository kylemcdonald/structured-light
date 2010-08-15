#include "PhaseDecoder.h"

PhaseDecoder::PhaseDecoder() :
		sequenceSize(0),
		gamma(1.0),
		colorSequence(NULL),
		graySequence(NULL),
		reflectivity(NULL),
		phase(NULL),
		ready(NULL),
		depthScale(1),
		depthSkew(1),
		maxPasses(1),
		minRemaining(0),
		color(NULL),
		orientation(PHASE_VERTICAL),
		phasePersistence(false),
		lastPhase(false),
		range(NULL),
		unwrapOrder(NULL) {
}

// This code should migrate to DepthDecoder, as it will
// be used for allocating a gray coded image sequence also.
void PhaseDecoder::setup(int width, int height, int sequenceSize) {
	this->width = width;
	this->height = height;
	this->sequenceSize = sequenceSize;

	colorSequence = new byte*[sequenceSize];
	graySequence = new byte*[sequenceSize];
	int n = width * height;
	for (int i = 0; i < sequenceSize; i++) {
		colorSequence[i] = new byte[n * 3];
		graySequence[i] = new byte[n];
	}
	reflectivity = new byte[n];
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
	if (colorSequence != NULL) {
		for (int i = 0; i < sequenceSize; i++) {
			delete [] colorSequence[i];
			delete [] graySequence[i];
		}
		delete [] colorSequence;
		delete [] graySequence;
		delete [] reflectivity;
		delete [] phase;
		delete [] wrappedPhase;
		delete [] ready;
		delete [] color;
		delete [] lastPhase;
		delete [] range;
	}
}

void PhaseDecoder::setMaxPasses(int maxPasses) {
	this->maxPasses = maxPasses;
}

void PhaseDecoder::setMinRemaining(float minRemaining) {
	this->minRemaining = minRemaining;
}

void PhaseDecoder::set(int position, byte* image, int channels) {
	byte* curColor = colorSequence[position];
	byte* curGray = graySequence[position];

	int n = width * height;
	int i = 0;
	int j = 0;
	if (channels == 3) {
		memcpy(curColor, image, n * 3);
		while (j < n) {
			float sum =
				(float) curColor[i++] +
				(float) curColor[i++] +
				(float) curColor[i++];
			sum /= 3;
			curGray[j++] = (byte) sum;
		}

		#ifdef LINEARIZE_GAMMA
		memset(gammaHistogram, 0, 256 * sizeof(int));
		for(i = 0; i < n; i++)
			gammaHistogram[curGray[i]]++;
		for(i = 1; i < 256; i++)
			gammaHistogram[i] += gammaHistogram[i - 1];
		for(i = 0; i < 256; i++)
			gammaHistogram[i] = (gammaHistogram[i] * 256) / n;
		for(i = 0; i < n; i++)
			curGray[i] = gammaHistogram[curGray[i]];
		#endif
	} else if(channels == 1) {
		while(i < n) {
			curColor[j++] = image[i];
			curColor[j++] = image[i];
			curColor[j++] = image[i];
			curGray[i] = image[i];
			i++;
		}
	}
}

void PhaseDecoder::decode() {
	makeColor();
	makePhase();
	memcpy( wrappedPhase, phase, sizeof(float) * width * height);
	for (int pass = 0; pass < maxPasses; pass++) {
		unwrapPhase();
		if (minRemaining != 0 && getRemaining() < minRemaining)
			break;
	}
	if (phasePersistence)
		memcpy(lastPhase, phase, sizeof(float) * width * height);
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
	int n = width * height;
	if (orientation == PHASE_VERTICAL) {
		float planeZero = (float) (startInd % width) / width;
		for (int i = 0; i < n; i++) {
			if (!mask[i]) {
				float x = (float) (i % width);
				float planephase = ((x / width) - planeZero) * depthSkew;
				depth[i] = (phase[i] - planephase) * depthScale;
			} else {
				depth[i] = 0;
			}
		}
	} else if (orientation == PHASE_HORIZONTAL) {
		float planeZero = 0;//(float) (startInd / width) / (float) height;
		for (int i = 0; i < n; i++) {
			if (!mask[i]) {
				float y = (float) (i / width);
				float planephase = ((y / height) - planeZero) * depthSkew;
				depth[i] = (phase[i] - planephase) * depthScale;
			} else {
				depth[i] = 0;
			}
		}
	}

	int offsetBins[offsetBinCount];
	memset(offsetBins, 0, offsetBinCount * sizeof(int));
	float offsetBinOffset = (offsetBinCount * offsetBinSize) / 2;
	for(int i = 0; i < n; i++) {
		if(!mask[i]) {
			int cur = (int) ((depth[i] + offsetBinOffset) / offsetBinSize);
			if(cur < 0)
				cur = 0;
			else if(cur >= offsetBinCount)
				cur = offsetBinCount - 1;
			offsetBins[cur]++;
		}
	}

	int biggestBin = 0;
	int biggestBinSize = 0;
	for(int i = 0; i < offsetBinCount; i++) {
		if(offsetBins[i] > biggestBinSize) {
			biggestBinSize = offsetBins[i];
			biggestBin = i;
		}
	}

	float offset = (biggestBin * offsetBinSize) - offsetBinOffset;

	for(int i = 0; i < n; i++)
		depth[i] -= offset;
}

byte* PhaseDecoder::getColor() {
	return color;
}

byte** PhaseDecoder::getGraySequence() {
	return graySequence;
}

float PhaseDecoder::getRemaining() {
	int n = width * height;
	int readySum = 0;
	int maskSum = 0;
	for (int i = 0; i < n; i++) {
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
	for (int i = 0; i < n; i++)
		if (sum[i] > sum[max])
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

void PhaseDecoder::exportTexture(string filename) {
	DepthExporter::exportTexture(filename, width, height, mask, color);
}
