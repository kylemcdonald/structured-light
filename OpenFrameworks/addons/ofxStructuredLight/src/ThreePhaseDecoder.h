#pragma once

#include "FloodFillDecoder.h"
#include "PriorityDecoder.h"

#define findRange(a, b, c) \
		(a < b ? \
			(b < c ? c - a : \
			(a < c ? b - a : \
				(b - c))) : \
			(c < b ? a - c : \
			(a < c ? c - b : \
				(a - b))))

//#define USE_GAMMA
//#define ACCURATE_COLOR
#define LINEARIZE_PHASE

/*
	Right now the decoders use decoding strategies via inheritance.
	This should be changed, so that PriorityDecoder extends
	DecodingStrategy while ThreePhaseDecoder is just a phase generator
	and color generator.
*/
class ThreePhaseDecoder : public FloodFillDecoder {
public:
	ThreePhaseDecoder();
	~ThreePhaseDecoder();
	void setup(int width, int height);
	float* getRange();
	void setRangeThreshold(float rangeThreshold);
	void setBrightness(float brightness);
	#ifdef USE_GAMMA
	float Gamma(float x, float gamma);
	float oldGamma;
	#endif

	#ifdef LINEARIZE_PHASE
	void setLinearize(bool linearize);
	int getLutWidth() const;
	int getLutHeight() const;
	const float* getLut(int i) const;
	#endif

protected:
	void makePhase();
	void unwrapPhase();
	void makeColor();

	float brightness;
	float rangeThreshold;

	#ifdef USE_GAMMA
	static const int LUT_SIZE = 16384;
	float gammaLut[LUT_SIZE];
	#endif

	#ifdef LINEARIZE_PHASE
	bool linearize;

	static const int lutWidth = 128;
	static const int lutHeight = 16;
	float lut[lutHeight][lutWidth];

	void buildLut();
	void applyLut();
	#endif
};
