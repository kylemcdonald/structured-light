#include "ThreePhaseDecoder.h"

float ThreePhaseDecoder::rangeThreshold = 20;

ThreePhaseDecoder::ThreePhaseDecoder() {
	range = NULL;
}

ThreePhaseDecoder::~ThreePhaseDecoder() {
	delete [] range;
}

void ThreePhaseDecoder::setup(int width, int height) {
	PhaseDecoder::setup(width, height, 3);
	range = new float[width * height];
}

void ThreePhaseDecoder::makePhase() {
	int n = width * height;
	float i1, i2, i3;
	for(int i = 0; i < n; i++) {
		i1 = (float) graySequence[0][i];
		i2 = (float) graySequence[1][i];
		i3 = (float) graySequence[2][i];
		phase[i] = atan2f(sqrtf(3) * (i1 - i3), 2.f * i2 - i1 - i3) / (float) TWO_PI;
		range[i] = range(i1, i2, i3);
		mask[i] = range[i] < rangeThreshold;
		ready[i] = !mask[i];
	}
}

void ThreePhaseDecoder::setGamma(float gamma) {
	this->gamma = gamma;
}

void ThreePhaseDecoder::makeColor() {
	int n = width * height * 3;
	float i1, i2, i3, avg;
	for(int i = 0; i < n; i++) {
		i1 = (float) colorSequence[0][i];
		i2 = (float) colorSequence[1][i];
		i3 = (float) colorSequence[2][i];
		avg = (i1 + i2 + i3) / 3;
		color[i] = (byte) avg;
	}
}

float* ThreePhaseDecoder::getRange() {
	return range;
}

void ThreePhaseDecoder::setRangeThreshold(float rangeThreshold) {
	this->rangeThreshold = rangeThreshold;
}

void ThreePhaseDecoder::unwrapPhase() {
	FloodFillDecoder::unwrapPhase();
}
