#ifndef _VISION_DIY3DSCAN_SRC_ThreePhaseDecoder
#define _VISION_DIY3DSCAN_SRC_ThreePhaseDecoder

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

#include "libexport.h"

/*
	Right now the decoders use decoding strategies via inheritance.
	This should be changed, so that PriorityDecoder extends
	DecodingStrategy while ThreePhaseDecoder is just a phase generator
	and color generator.
*/
class DLL_EXPORT ThreePhaseDecoder : public PriorityDecoder {
public:
	ThreePhaseDecoder();
	~ThreePhaseDecoder();
	void setup(int width, int height);
	float* getRange();
	void setRangeThreshold(float rangeThreshold);
	float getRangeThreshold();
protected:
	void makePhase();
	void unwrapPhase();
	void makeColor();
	float* range;
	static float rangeThreshold;
};

#endif
