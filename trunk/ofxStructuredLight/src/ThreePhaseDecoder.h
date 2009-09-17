#pragma once

#include "FloodFillDecoder.h"

#ifndef range
	#define range(a, b, c) \
		(a < b ? \
			(b < c ? c - a : \
			(a < c ? b - a : \
				(b - c))) : \
			(c < b ? a - c : \
			(a < c ? c - b : \
				(a - b))))
#endif

class ThreePhaseDecoder : public FloodFillDecoder {
public:
	ThreePhaseDecoder();
	~ThreePhaseDecoder();
	void setup(int width, int height);
	float* getRange();
	void setRangeThreshold(float rangeThreshold);
	void setGamma(float gamma);
protected:
	void makePhase();
	void unwrapPhase();
	void makeColor();
	float* range;
	float gamma;
	static float rangeThreshold;
};
