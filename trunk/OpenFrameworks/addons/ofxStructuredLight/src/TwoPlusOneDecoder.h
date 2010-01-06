#pragma once

#include "FloodFillDecoder.h"

class TwoPlusOneDecoder : public FloodFillDecoder {
public:
	void setup(int width, int height);
protected:
	void makePhase();
	void unwrapPhase();
	void makeColor();
};
