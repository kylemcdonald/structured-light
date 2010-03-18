#include "ThreePhaseDecoder.h"

ThreePhaseDecoder::ThreePhaseDecoder() :
	brightness(.8),
	rangeThreshold(20) {
}

ThreePhaseDecoder::~ThreePhaseDecoder() {
}

void ThreePhaseDecoder::setup(int width, int height) {
	PhaseDecoder::setup(width, height, 3);
	range = new float[width * height];
	unwrapOrder = new float[width * height];
}

float ThreePhaseDecoder::Gamma(float x, float gamma) {
	if (gamma != oldGamma) {
		for (int i = 0; i < LUT_SIZE; i++) {
			float x = (float)i / (float)LUT_SIZE;
			if (x < 0.5) {
				gammaLut[i] =  0.5 * pow(2 * x , gamma);
			} else {
				/// could cut the size of the lut in half  with a little more work
				gammaLut[i] = 1.0 - 0.5 * pow(2 * (1 - x), gamma);
			}
		}
	}
	oldGamma = gamma;

	if (x < 0) return gammaLut[0];
	if (int(x*LUT_SIZE) >= LUT_SIZE) return gammaLut[LUT_SIZE-1];

	return gammaLut[int(x*LUT_SIZE)];
}


void ThreePhaseDecoder::makePhase() {
	int n = width * height;
	float i1, i2, i3;
	for (int i = 0; i < n; i++) {
		i1 = (float) graySequence[0][i];
		i2 = (float) graySequence[1][i];
		i3 = (float) graySequence[2][i];

		i1 = Gamma(i1 / 255.0, gamma) * 255.0;
		i2 = Gamma(i2 / 255.0, gamma) * 255.0;
		i3 = Gamma(i3 / 255.0, gamma) * 255.0;

		range[i] = findRange(i1, i2, i3);
		mask[i] = range[i] <= rangeThreshold;
		ready[i] = !mask[i];

		if(ready[i])
			phase[i] = atan2f(sqrtf(3) * (i1 - i3), 2.f * i2 - i1 - i3) / (float) TWO_PI;
	}
}

void ThreePhaseDecoder::makeColor() {
	int n = width * height * 3;
	float i1, i2, i3, a, b, c;
	for (int i = 0; i < n; i++) {
		i1 = (float) colorSequence[0][i];
		i2 = (float) colorSequence[1][i];
		i3 = (float) colorSequence[2][i];
		// This comes from "Recent progresses on real-time 3D shape measurement..."
		// The scaling factor "brightness" is due to i1,i2,i3 not necessarily being
		// perfectly sampled from out of phase cosines.
		a = i1 - i3;
		b = 2 * i2 - i1 - i3;
		c = (i1 + i2 + i3 + sqrtf(3 * a * a + b * b)) / 3;
		color[i] = (byte) (c * brightness);
	}
}

float* ThreePhaseDecoder::getRange() {
	return range;
}

void ThreePhaseDecoder::setRangeThreshold(float rangeThreshold) {
	this->rangeThreshold = rangeThreshold;
}

void ThreePhaseDecoder::setBrightness(float brightness) {
	this->brightness = brightness;
}

void ThreePhaseDecoder::unwrapPhase() {
	FloodFillDecoder::unwrapPhase();
}
