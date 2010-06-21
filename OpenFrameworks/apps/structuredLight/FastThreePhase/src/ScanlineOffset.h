#pragma once

/*
	Second chances are not specified by the original paper, and it makes things
	slightly slower... but provides a more complete unwrapping for higher gamma
	thresholds.
*/
#define GIVE_SECOND_CHANCES

/*
	In the original paper this is given as 3, but for systems with more noise
	it is helpful to boost this a bit. This parameter offers a speed/accuracy
	tradeoff. Lower values are faster but less accurate.
*/
#define LEVEL_COUNT 6

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

	int levelCutoff[LEVEL_COUNT];
	void makeLevelCutoff();

	int* leftover;
	int leftoverSize;
	void unwrapPatch(
		int start, unsigned char curLevel,
		int xlen, int ylen, int xinside, int yinside);
	void processLeftover(int xoff, int yoff);
	char unwrapPhase(unsigned char source, unsigned char target);

	static const int farthestNeighbor = 128;
	vector<int> neighbors;
};

class DistanceIndex {
public:
	float distance;
	int index;
	bool operator<(const DistanceIndex& b) const {
		return distance < b.distance;
	}
};
