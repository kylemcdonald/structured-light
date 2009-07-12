#pragma once

#include "ofImage.h"
#include <deque>
#include <iostream>
using namespace std;

/*
	Possible optimizations:
	- working with *s instead of **s
	- replacing toProcess with a fixed-capacity deque
	-using an int[2] instead of intPoint

	Next:
	- how long does decoding take?
	- capture + playback three phase video
*/

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

	deque<intPoint> toProcess;

	float** wrapphase;
	bool** mask;
	bool** process;
};
