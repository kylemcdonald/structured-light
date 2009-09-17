#pragma once

#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
using namespace std;

#ifndef TWO_PI
	#define TWO_PI (6.28318530718)
#endif

typedef unsigned char byte;

class PhaseDecoder {
public:
	PhaseDecoder();
	int getWidth();
	int getHeight();
	void setDepthScale(float depthScale);
	void setDepthSkew(float depthSkew);
	void set(int i, byte* image);
	void decode();
	float* getPhase();
	bool* getMask();
	float* getDepth();
	byte* getColor();
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
	float depthScale, depthSkew;

	void setup(int width, int height, int sequenceSize);
	virtual void makePhase() = 0;
	virtual void unwrapPhase() = 0;
	void makeDepth();
	virtual void makeColor() = 0;
};
