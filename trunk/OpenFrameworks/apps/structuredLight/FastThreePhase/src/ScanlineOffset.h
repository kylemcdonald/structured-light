#pragma once

/*
	Second chances are not specified by the original paper, and it makes things
	slightly slower... but provides a more complete unwrapping for higher gamma
	thresholds.
*/
//#define GIVE_SECOND_CHANCES

#include "Shared.h"

class ScanlineOffset {
public:
	ScanlineOffset();
	~ScanlineOffset();
	void setup(int width, int height);
	void setThreshold(unsigned char threshold);
	void makeOffset(
		unsigned char* phase,
		unsigned char* quality,
		char* offset);
	int* getQualityHistogram();
	int* getLevelCutoff();
private:
	int width, height, n;
	int qualityHistogram[256];

	unsigned char threshold;

	unsigned char *phase, *quality;
	char *offset;

	#define LEVEL_COUNT 3
	int levelCutoff[LEVEL_COUNT];
	void makeLevelCutoff();

	int* leftover;
	int leftoverSize;
	void unwrapPatch(
		int start, unsigned char curLevel,
		int xlen, int ylen, int xinside, int yinside);
	void processLeftover(int xoff, int yoff);
	char unwrapPhase(unsigned char source, unsigned char target);
};
