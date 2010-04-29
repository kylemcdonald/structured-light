#include "ScanlineOffset.h"

ScanlineOffset::ScanlineOffset() :
	threshold(LABEL_BACKGROUND) {
}

ScanlineOffset::~ScanlineOffset() {
	delete [] leftover;
}

void ScanlineOffset::setup(int width, int height) {
	this->width = width;
	this->height = height;
	n = width * height;
	leftover = new int[n];
}

void ScanlineOffset::setThreshold(unsigned char threshold) {
	this->threshold = threshold;
}

void ScanlineOffset::makeOffset(
		unsigned char* phase,
		unsigned char* quality,
		char* offset) {
	this->phase = phase;
	this->quality = quality;
	this->offset = offset;

	makeLevelCutoff();

	// label all points
	// (compiler should unroll this when LEVEL_COUNT is constant
	// but generally, it would be faster with the loops switched)
	for(int i = 0; i < n; i++) {
		if(quality[i] < threshold) {
			for(int j = 0; j < LEVEL_COUNT; j++)
				if(quality[i] < levelCutoff[j]) {
					quality[i] = j;
					break;
				}
		} else {
			quality[i] = LABEL_BACKGROUND;
		}
	}

	// pick starting point
	int startX = width / 2;
	int startY = height / 2;
	int start = startY * width + startX;
	if(quality[start] != 0) {
		// right now, just give up if the middle isn't a good place to start
		// this needs to be replaced by code that finds a good starting point
		return;
	} else
		quality[start] = LABEL_UNWRAPPED;

	offset[start] = 0; // center start
	for(unsigned char curLevel = 0; curLevel < LEVEL_COUNT; curLevel++) {
		unwrapPatch(start, curLevel, startX - 1, startY - 1, +1, +width); // nw
		unwrapPatch(start, curLevel, width - startX - 1, startY - 1, -1, +width); // ne
		unwrapPatch(start, curLevel, startX - 1, height - startY - 1, +1, -width); // sw
		unwrapPatch(start, curLevel, width - startX - 1, height - startY - 1, -1, -width); // se
	}
}

int* ScanlineOffset::getQualityHistogram() {
	return qualityHistogram;
}

int* ScanlineOffset::getLevelCutoff() {
	return levelCutoff;
}

void ScanlineOffset::makeLevelCutoff() {
	// clear and build histogram
	memset(qualityHistogram, 0, 256 * sizeof(int));
	for(int i = 0; i < n; i++)
		qualityHistogram[quality[i]]++;
	qualityHistogram[LABEL_BACKGROUND] = 0; // background doesn't count

	// get mean quality
	int sumQuality = 0;
	int sumQualityWeights = 0;
	for(int i = 0; i < 256; i++) {
		sumQuality += qualityHistogram[i] * i;
		sumQualityWeights += qualityHistogram[i];
	}
	int meanQuality = sumQuality / sumQualityWeights;

	// get std dev
	float sumSqDiff = 0;
	for(int i = 0; i < 256; i++) {
		int cur = i - meanQuality;
		sumSqDiff += qualityHistogram[i] * cur * cur;
	}
	float stdDev = sqrtf(sumSqDiff / (sumQualityWeights - 1));

	// pick level cutoffs
	for(int i = 0; i < LEVEL_COUNT; i++)
		levelCutoff[i] = (int) (meanQuality + ((1 << i) >> 1) * stdDev);
	levelCutoff[LEVEL_COUNT - 1] = LABEL_BACKGROUND;
}

inline void ScanlineOffset::unwrapPatch(
		int start, unsigned char curLevel,
		int xlen, int ylen, int xinside, int yinside) {
	leftoverSize = 0;
	int i = start;
	int xoff = xinside * xlen;
	int closer;
	for(int y = 0; y < ylen; y++) {
		for(int x = 0; x < xlen; x++) {
			if(quality[i]
					#ifdef GIVE_SECOND_CHANCES
					<=
					#else
					==
					#endif
					curLevel) {
				closer = i + xinside;
				if(quality[closer] == LABEL_UNWRAPPED) {
					// unwrap from left
					offset[i] = offset[closer] + unwrapPhase(phase[closer], phase[i]);
					quality[i] = LABEL_UNWRAPPED;
				} else {
					closer = i + yinside;
					if(quality[closer] == LABEL_UNWRAPPED) {
						// unwrap from below
						offset[i] = offset[closer] + unwrapPhase(phase[closer], phase[i]);
						quality[i] = LABEL_UNWRAPPED;
					} else { // can't unwrap from start-facing pixels
						if(quality[i - xinside] != LABEL_BACKGROUND ||
								quality[i - yinside] != LABEL_BACKGROUND) {
							// if it has a valid border-facing pixel, add to leftovers
							leftover[leftoverSize++] = i;
						}
					}
				}
			}
			i -= xinside;
		}
		i -= yinside;
		i += xoff;
	}
	processLeftover(-xinside, -yinside);
}

void ScanlineOffset::processLeftover(int xoff, int yoff) {
	int i, farther;
	for(int j = leftoverSize - 1; j >= 0; j--) {
		i = leftover[j];
		farther = i + xoff;
		if(quality[farther] == LABEL_UNWRAPPED) {
			offset[i] = offset[farther] + unwrapPhase(phase[farther], phase[i]);
			quality[i] = LABEL_UNWRAPPED;
		} else {
			farther = i + yoff;
			if(quality[farther] == LABEL_UNWRAPPED) {
				offset[i] = offset[farther] +	unwrapPhase(phase[farther], phase[i]);
				quality[i] = LABEL_UNWRAPPED;
			} else {
				// isn't a background pixel, and has non-background neighbors,
				// but none of these things were ever unwrapped
				// (these could potentially be added to a final pass
				// that attempts to unwrap any remaining pixels)
			}
		}
	}
}

inline char ScanlineOffset::unwrapPhase(unsigned char source, unsigned char target) {
	if(source < target) {
		if(target - source < 128) {
			// source <128 target
			return 0; // no phase change
		} else {
			// source >128 target
			return -1; // lower phase
		}
	} else {
		if(source - target < 128) {
			// target <128 source
			return 0; // no phase change
		} else {
			// target >128 source
			return +1; // next phase
		}
	}
}
