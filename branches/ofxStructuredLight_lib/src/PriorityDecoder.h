#pragma once

#include "PhaseDecoder.h"

#include <queue>

#ifndef abs
	#define abs(x) (x < 0 ? -x : x)
#endif

#include "libexport.h"

class DLL_EXPORT UnwrapPath {
public:
	float diff;
	int target;
	float resultPhase;
	UnwrapPath(float _diff, int _target, float _resultPhase) :
		diff(_diff), target(_target), resultPhase(_resultPhase) {
	};
	bool operator<(const UnwrapPath& path) const {
		return abs(diff) > abs(path.diff);
	}
};

class DLL_EXPORT PriorityDecoder : public PhaseDecoder {
protected:
	virtual void unwrapPhase();
	void unwrapPhase(int target, float sourcePhase);
	std::priority_queue<UnwrapPath, std::vector<UnwrapPath>, std::less<UnwrapPath> > toProcess;
};
