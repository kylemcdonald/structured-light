#include "PatternGenerator.h"

void PatternGenerator::allocateSequence(int n) {
	sequence.clear();
	for(int i = 0; i < n; i++) {
		sequence.push_back(ofImage());
		sequence.back().allocate(width, height, OF_IMAGE_COLOR);
	}
}

PatternGenerator::PatternGenerator() :
	width(0), height(0),
	reverse(false) {
}

void PatternGenerator::applyLut(string filename) {
	ifstream file;
	file.open(filename.c_str());
	if(!file.is_open())
		cout << "Can't load LUT: " << filename << endl;
	int lut[3][256];
	string line;
	int i = 0;
	while(getline(file, line)) {
		istringstream cur(line);
		cur >> lut[0][i] >> ws >> lut[1][i] >> ws >> lut[2][i];
		i++;
	}
	file.close();

	for(int i = 0; i < size(); i++) {
		ofImage& cur = sequence[i];
		unsigned char* pixels = cur.getPixels();
		int n = 3 * getWidth() * getHeight();
		for(int j = 0; j < n;) {
			pixels[j] = lut[0][pixels[j]];
			j++;
			pixels[j] = lut[1][pixels[j]];
			j++;
			pixels[j] = lut[2][pixels[j]];
			j++;
		}
		cur.update();
	}
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

ofImage& PatternGenerator::get(int i) {
	i %= size();
	if(reverse) {
		return sequence[size() - i - 1];
	} else {
		return sequence[i];
	}
}
