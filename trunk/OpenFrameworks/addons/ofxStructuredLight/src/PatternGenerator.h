#pragma once

#include "ofMain.h"
#include <fstream>

class PatternGenerator {
protected:
	bool reverse;
	int width, height;
	vector<ofImage> sequence;
	void allocateSequence(int n);
public:
	PatternGenerator();
	void setSize(int width, int height);
	void setReverse(bool reverse);
	virtual void generate() = 0;
	int getWidth();
	int getHeight();
	int size();
	void applyLut(string filename);
	ofImage& get(int i);
};
