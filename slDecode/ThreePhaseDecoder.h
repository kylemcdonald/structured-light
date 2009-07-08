#pragma once

#include "ofImage.h"
#include <deque>
#include <iostream>
using namespace std;

struct int2 {
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
	float** getWrapPhase();
	void decode();
	~ThreePhaseDecoder();
	ofImage phase1Image, phase2Image, phase3Image;
private:
	void phaseUnwrapAll();
	float phaseUnwrap(float phase1, float phase2, float phase3);
	void propagatePhases();
	void unwrap(float r, int x, int y);

	float noiseTolerance;
	int width, height;

	deque<int2> toProcess;

	float** _wrapphase;
	bool** _mask;
	bool** _process;
};
