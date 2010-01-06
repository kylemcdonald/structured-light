#pragma once

#include "ofImage.h"
#include <vector>

enum generatorOrientation {
	GENERATOR_HORIZONTAL,
	GENERATOR_VERTICAL};

class PatternGenerator {
public:
	void generate(int width, int height) {
		this->width = width;
		this->height = height;
		_generate();
	}
	ofImage& get(int i) {
		return sequence[i];
	}
	int size() {
		return sequence.size();
	}
	int getWidth() {
		return width;
	}
	int getHeight() {
		return height;
	}
protected:
	int width, height;
	vector<ofImage> sequence;

	virtual void _generate() = 0;
};
