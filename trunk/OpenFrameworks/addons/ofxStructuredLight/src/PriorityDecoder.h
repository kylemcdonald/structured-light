#pragma once

#include "PhaseDecoder.h"
#include <queue>

#ifndef abs
#define abs(x) (x < 0 ? -x : x)
#endif

class WrappedPixel {
public:
	int target;
	float distance, phase;
	WrappedPixel(int target, float distance, float phase) :
			target(target), phase(phase), distance(distance) {
	}
	bool operator<(const WrappedPixel& wrappedPixel) const {
		return distance < wrappedPixel.distance;
	}
};

class PriorityDecoder : public PhaseDecoder {
public:
	float maxPhase;
	float minPhase;

	PriorityDecoder();
	~PriorityDecoder();

protected:
	virtual void unwrapPhase();

	float* distance;
	float diff(float a, float b);
	void unwrapPhase(int target, float d, float r);
	std::priority_queue<WrappedPixel, std::vector<WrappedPixel>, std::less<WrappedPixel> > toProcess;
};
