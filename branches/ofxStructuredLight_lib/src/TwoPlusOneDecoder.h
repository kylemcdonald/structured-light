#ifndef _VISION_DIY3DSCAN_SRC_TwoPlusOneDecoder
#define _VISION_DIY3DSCAN_SRC_TwoPlusOneDecoder

#include "FloodFillDecoder.h"

#include "libexport.h"

class DLL_EXPORT TwoPlusOneDecoder : public FloodFillDecoder {
public:
	void setup(int width, int height);
protected:
	void makePhase();
	void unwrapPhase();
	void makeColor();
};

#endif
