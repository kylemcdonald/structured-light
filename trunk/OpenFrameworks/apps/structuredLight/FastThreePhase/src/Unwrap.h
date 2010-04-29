#pragma once

#include "Shared.h"

class Unwrap {
public:
	Unwrap();
	~Unwrap();
	void setup(int width, int height);
	void makeUnwrapped(unsigned char* quality, unsigned char* phase, char* offset);
	void makeUnwrapped(unsigned char* quality, unsigned char* phase, char* offset, unsigned char* unwrappedPhaseImage);
	int* getUnwrapped();
private:
	int width, height, n;
	int* unwrappedPhase;
	int minPhase, maxPhase;
};
