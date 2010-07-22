#include "PartialQualityMap.h"

#ifdef USE_QUALITY_LUT
bool PartialQualityMap::qualityLutReady = false;
unsigned char PartialQualityMap::qualityLut[QUALITY_LUT_STEPS][QUALITY_LUT_STEPS][QUALITY_LUT_STEPS];
#endif

PartialQualityMap::PartialQualityMap() {
	#ifdef USE_QUALITY_LUT
	if(!qualityLutReady) {
		cout << "Building quality lut... ";
		unsigned char fromd, tod;
		for(int cur = 0; cur < QUALITY_LUT_STEPS; cur++) {
			for(int from = 0; from < QUALITY_LUT_STEPS; from++) {
				for(int to = 0; to < QUALITY_LUT_STEPS; to++) {
					fromd = wrapQualityDifference(cur, from);
					tod = wrapQualityDifference(to, cur);
					qualityLut[cur][from][to] = fromd > tod ? fromd : tod;
				}
			}
		}
		cout << "done." << endl;
		qualityLutReady = true;
	}
	#endif
}

void PartialQualityMap::setup(int width, int height) {
	this->width = width;
	this->height = height;
}

void PartialQualityMap::makeQualityMap(const unsigned char* phase, unsigned char* quality) {
	unsigned char cur;
	#ifdef USE_QUALITY_LUT
	unsigned char alt;
	#else
	unsigned char wd, ed, nd, sd;
	#endif
	int i = width; // skip first row
	for(int y = 1; y < height - 1; y++) {
		i++; // skip first pixel
		for(int x = 1; x < width - 1; x++) {
			cur = phase[i];
			if(quality[i] != LABEL_BACKGROUND) {
				#ifdef USE_QUALITY_LUT
				cur >>= QUALITY_LUT_ACCURACY;
				quality[i] = qualityLut
					[cur]
					[phase[i - 1] >> QUALITY_LUT_ACCURACY]
					[phase[i + 1] >> QUALITY_LUT_ACCURACY];
				alt = qualityLut
					[cur]
					[phase[i - width] >> QUALITY_LUT_ACCURACY]
					[phase[i + width] >> QUALITY_LUT_ACCURACY];
				if(alt > quality[i])
					quality[i] = alt;
				#else
				// make partial derivatives
				wd = wrapQualityDifference(cur, phase[i - 1]);
				ed = wrapQualityDifference(phase[i + 1], cur);
				nd = wrapQualityDifference(cur, phase[i - width]);
				sd = wrapQualityDifference(phase[i + width], cur);

				// find max partial derivative
				quality[i] = wd;
				if(ed > quality[i])
					quality[i] = ed;
				if(nd > quality[i])
					quality[i] = nd;
				if(sd > quality[i])
					quality[i] = sd;
				#endif
			}
			i++; // next pixel
		}
		i++; // skip final pixel
	}
}

// absolute value of a wrapped partial derivative, |W{a - b}|
inline unsigned char PartialQualityMap::wrapQualityDifference(unsigned char a, unsigned char b) {
	unsigned char inside = a - b; // same as (a < b ? b - a : a - b) here due to wrapping
	#ifdef USE_QUALITY_LUT
	inside <<= QUALITY_LUT_ACCURACY;
	#endif
	return inside < 128 ? inside : 256 - inside;
}
