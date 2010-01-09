#include "PatternGenerator.h"

void PatternGenerator::allocateSequence(int n) {
	sequence.clear();
	for(int i = 0; i < n; i++) {
#ifdef OPENFRAMEWORKS_AVAIL
		sequence.push_back(ofImage());
		sequence.back().allocate(width, height, OF_IMAGE_COLOR);
#else
		IplImage* im = cvCreateImage(cvSize(width, height), 8, 3);
		if (im) sequence.push_back(*im);
#endif
	}
}

PatternGenerator::PatternGenerator() :
	width(0), height(0),
	reverse(false) {
}

void PatternGenerator::setSize(int width, int height) {
	this->width = width;
	this->height = height;
}

void PatternGenerator::setReverse(bool reverse) {
	this->reverse = reverse;
}

int PatternGenerator::getWidth() {
	return width;
}

int PatternGenerator::getHeight() {
	return height;
}

int PatternGenerator::size() {
	return sequence.size();
}

#ifdef OPENFRAMEWORKS_AVAIL
ofImage& PatternGenerator::get(int i) {
#else
IplImage& PatternGenerator::get(int i) {
#endif
	i %= size();
	if(reverse) {
		return sequence[size() - i - 1];
	} else {
		return sequence[i];
	}
}
