#pragma once

#include "FloodFillDecoder.h"
#include "PriorityDecoder.h"

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

/*
	Right now the decoders use decoding strategies via inheritance.
	This should be changed, so that PriorityDecoder extends
	DecodingStrategy while ThreePhaseDecoder is just a phase generator
	and color generator.
*/
class ThreePhaseDecoder : public PriorityDecoder {
public:
	ThreePhaseDecoder();
	~ThreePhaseDecoder();
	void setup(int width, int height, int numColorChan);
	float* getRange();
	void setRangeThreshold(float rangeThreshold);
	float Gamma(float x, float gamma);
	float oldGamma;



protected:
	void makePhase();
	void unwrapPhase();
	void makeColor();



	float* range;
	static float rangeThreshold;

    static const int LUT_SIZE = 16384;
	float gammaLut[LUT_SIZE];
};
