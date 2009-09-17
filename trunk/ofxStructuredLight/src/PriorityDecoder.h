#pragma once

#include "PhaseDecoder.h"
#include <queue>

class PriorityDecoder : public PhaseDecoder {
	virtual void unwrapPhase();
	inline void unwrapPhase(int target, float sourcePhase);
	std::priority_queue<int> toProcess;
};
