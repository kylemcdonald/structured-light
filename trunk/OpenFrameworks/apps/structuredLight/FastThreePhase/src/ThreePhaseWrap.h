#pragma once

/*
	Using a wrap LUT increases the speed of the three phase wrapping by
	pre-computing the sqrtf and gamma for every three phase combination. It offers
	a significant speed increase when WRAP_LUT_ACCURACY = 0, but requires a lot of
	memory. If you're willing to sacrifice some accuracy, WRAP_LUT_ACCURACY = 1
	offers a good compromise with speed.
*/
//#define USE_WRAP_LUT

#include "ImageUtility.h"
#include "Shared.h"

class ThreePhaseWrap {
public:
	ThreePhaseWrap();
	~ThreePhaseWrap();
	void setup(int width, int height);
	void setThreshold(unsigned char threshold);
	void set(int phase, const unsigned char* image, int channels = 1);
	unsigned char* get(int phase);
	void makeWrappedPhase(unsigned char* wrappedPhase, unsigned char* mask);
private:
	int	width, height, n;
	unsigned char threshold;
	unsigned char* phaseImage[3];

	#ifdef USE_WRAP_LUT
	#define WRAP_LUT_ACCURACY 1
	#define WRAP_LUT_STEPS (256 >> WRAP_LUT_ACCURACY)
	static bool phaseGammaLutReady;
	static unsigned char phaseGammaLut[WRAP_LUT_STEPS][WRAP_LUT_STEPS][WRAP_LUT_STEPS][2];
	#endif
};
