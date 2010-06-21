#pragma once

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

/*
	Using a LUT on the wrapping significantly speeds things up by avoiding the
	repeated sqrtf and atan2f calls. It uses the v and u values as idices to an
	array for phase and data modulation values (used here as a substitute for
	gamma).

	This technique is also described in the 2010 paper "Dual-frequency pattern
	scheme for high-speed 3-D shape measurement" by Liu et al.
*/
	#define WRAP_V_SIZE 511 // i1 - i3, -255 to 255, |511|
	#define WRAP_U_SIZE 1021 // 2 * i2 - i1 - i3, -510 to 510, |1021|
	#define WRAP_V_OFFSET 255 // min value is -255
	#define WRAP_U_OFFSET 510 // min value is -510
	static bool phaseGammaLutReady;
	static unsigned char phaseGammaLut[WRAP_V_SIZE][WRAP_U_SIZE][2];
};
