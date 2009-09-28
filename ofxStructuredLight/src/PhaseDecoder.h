#pragma once

#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include "FastBlur.h"
using namespace std;

#ifndef TWO_PI
	#define TWO_PI (6.28318530718)
#endif

typedef unsigned char byte;
enum phaseOrientation {PHASE_VERTICAL, PHASE_HORIZONTAL};

class PhaseDecoder {
public:
	PhaseDecoder();
	int getWidth();
	int getHeight();
	void setDepthScale(float depthScale);
	void setDepthSkew(float depthSkew);
	void setOrientation(phaseOrientation orientation);
	void setMaxPasses(int maxPasses);
	void setMinRemaining(float minRemaining);
	void set(int i, byte* image);
	void decode();
	float* getPhase();
	bool* getMask();
	float* getDepth();
	byte* getColor();
	int* getBlur();
	void save(string filename);
	virtual ~PhaseDecoder();
protected:
	int width, height;
	byte** colorSequence;
	byte** graySequence;
	float* phase;
	bool* mask;
	bool* ready;
	float* depth;
	byte* color;
	int sequenceSize;
	int maxPasses;
	float minRemaining;
	float depthScale, depthSkew;
	phaseOrientation orientation;
	FastBlur blur;

	void setup(int width, int height, int sequenceSize);
	virtual void makePhase() = 0;
	virtual void unwrapPhase() = 0;
	void makeDepth();
	virtual void makeColor() = 0;
	virtual int getStart();
	float getRemaining();
};
