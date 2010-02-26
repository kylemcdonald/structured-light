#include "TwoPlusOneDecoder.h"

void TwoPlusOneDecoder::setup(int width, int height) {
	PhaseDecoder::setup(width, height, 3);
}

void TwoPlusOneDecoder::makePhase() {
	int n = width * height;
	float i1, i2, i3;
	for(int i = 0; i < n; i++) {
		i1 = (float) graySequence[0][i];
		i2 = (float) graySequence[1][i];
		i3 = (float) graySequence[2][i];
		phase[i] = atan2f(i1 - i3, i2 - i3) / (float) TWO_PI;
		mask[i] = false;
		ready[i] = !mask[i];
	}
}

void TwoPlusOneDecoder::unwrapPhase() {
	FloodFillDecoder::unwrapPhase();
}

void TwoPlusOneDecoder::makeColor() {
}
