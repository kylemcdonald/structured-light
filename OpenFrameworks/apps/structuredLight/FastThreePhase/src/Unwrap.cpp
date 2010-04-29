#include "Unwrap.h"

Unwrap::Unwrap() {
}

Unwrap::~Unwrap() {
	delete [] unwrappedPhase;
}

void Unwrap::setup(int width, int height) {
	this->width = width;
	this->height = height;
	n = width * height;
	unwrappedPhase = new int[n];
}

void Unwrap::makeUnwrapped(
		unsigned char* quality,
		unsigned char* phase,
		char* offset) {
	minPhase = 0;
	maxPhase = 0;
	for(int i = 0; i < n; i++) {
		if(quality[i] == LABEL_UNWRAPPED) {
			unwrappedPhase[i] = offset[i];
			unwrappedPhase[i] *= 256;
			unwrappedPhase[i] += phase[i];
			if(unwrappedPhase[i] < minPhase)
				minPhase = unwrappedPhase[i];
			if(unwrappedPhase[i] > maxPhase)
				maxPhase = unwrappedPhase[i];
		} else {
			unwrappedPhase[i] = 0;
		}
	}
}

void Unwrap::makeUnwrapped(
		unsigned char* quality,
		unsigned char* phase,
		char* offset,
		unsigned char* unwrappedPhaseImage) {
	makeUnwrapped(quality, phase, offset);
	int rangeUnwrapped = maxPhase - minPhase;
	if(rangeUnwrapped != 0)
		for(int i = 0; i < n; i++)
			unwrappedPhaseImage[i] = 255 * (unwrappedPhase[i] - minPhase) / rangeUnwrapped;
}

int* Unwrap::getUnwrapped() {
	return unwrappedPhase;
}
