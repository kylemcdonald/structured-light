#pragma once

#include "PhaseDecoder.h"

#include <deque>

#include "libexport.h"

class DLL_EXPORT FloodFillDecoder : public PhaseDecoder {
protected:
	virtual void unwrapPhase();
	void unwrapPhase(int target, float sourcePhase);
	std::deque<int> toProcess;
};
