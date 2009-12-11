#ifndef _VISION_DIY3DSCAN_SRC_FloodFillDecoder
#define _VISION_DIY3DSCAN_SRC_FloodFillDecoder

#include "PhaseDecoder.h"

#include <deque>

#include "libexport.h"

class DLL_EXPORT FloodFillDecoder : public PhaseDecoder {
protected:
	virtual void unwrapPhase();
	void unwrapPhase(int target, float sourcePhase);
	std::deque<int> toProcess;
};

#endif
