#pragma once

/*
	Using a quality LUT is most helpful when it increases the quantization of the
	quality metric. E.g., when QUALITY_LUT_ACCURACY == 1, it approximately halves
	the runtime of makeQualityMap().
*/
//#define USE_QUALITY_LUT

#include "Shared.h"

class PartialQualityMap {
public:
	PartialQualityMap();
	void setup(int width, int height);
	void makeQualityMap(const unsigned char* phase, unsigned char* quality);
private:
	#ifdef USE_QUALITY_LUT
	#define QUALITY_LUT_ACCURACY 1
	#define QUALITY_LUT_STEPS (256 >> QUALITY_LUT_ACCURACY)
	static bool qualityLutReady;
	static unsigned char qualityLut[QUALITY_LUT_STEPS][QUALITY_LUT_STEPS][QUALITY_LUT_STEPS];
	#endif

	int width, height;

	unsigned char wrapQualityDifference(unsigned char a, unsigned char b);
};
