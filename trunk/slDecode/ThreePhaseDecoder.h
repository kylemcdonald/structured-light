#pragma once

#include "ofImage.h"
#include "FastQueue.h"

struct intPoint {
	int x;
	int y;
};

class ThreePhaseDecoder {
public:
	void setup(int width, int height, float noiseTolerance = .15);
	void loadImages();
	int getWidth();
	int getHeight();
	bool** getProcess();
	bool** getMask();
	float** getWrapPhase();
	void decode();
	~ThreePhaseDecoder();
	ofImage phase1Image, phase2Image, phase3Image;
private:
	void phaseUnwrapAll();
	inline float phaseUnwrap(
		const unsigned char& phase1,
		const unsigned char& phase2,
		const unsigned char& phase3);
	void propagatePhases();
	inline void unwrap(const float& r, int x, int y);

	float noiseTolerance;
	int width, height;

	FastQueue<intPoint> toProcess;

	float** wrapphase;
	bool** mask;
	bool** process;
};
