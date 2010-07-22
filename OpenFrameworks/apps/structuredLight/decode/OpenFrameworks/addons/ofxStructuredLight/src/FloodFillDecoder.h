#pragma once

#include "PhaseDecoder.h"
#include <deque>

class FloodFillDecoder : public PhaseDecoder {
protected:
	virtual void unwrapPhase();
	void unwrapPhase(int target, float sourcePhase);
	std::deque<int> toProcess;
};
