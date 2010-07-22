#include "GrayCodeGenerator.h"

GrayCodeGenerator::GrayCodeGenerator() :
	orientation(PHASE_VERTICAL),
	length(1024),
	subdivisions(10) {
}

void GrayCodeGenerator::setOrientation(phaseOrientation orientation) {
	this->orientation = orientation;
}

void GrayCodeGenerator::setLength(unsigned int length) {
	this->length = length;
}

void GrayCodeGenerator::setSubdivisions(unsigned int subdivisions) {
	this->subdivisions = subdivisions;
}

// for conversion: http://www.dspguru.com/dsp/tricks/gray-code-conversion
#define makeGrayCode(x) (x >> 1) ^ x
#define isTrue(x, i) (x >> i & 1)

void GrayCodeGenerator::generate() {
	allocateSequence(subdivisions);
	int maxSubdivisions = (int) log2(length);
	for(int k = maxSubdivisions - subdivisions; k < maxSubdivisions; k++) {
		unsigned char* single = new unsigned char[length * 3];
		int i = 0;
		for(unsigned int j = 0; j < length; j++) {
			unsigned int grayCode = makeGrayCode(j);
			unsigned char value = isTrue(grayCode, k) ? 255 : 0;
			single[i++] = value;
			single[i++] = value;
			single[i++] = value;
		}
		int sequencePosition = maxSubdivisions - k - 1;
		unsigned char* pixels = sequence[sequencePosition].getPixels();
		if(orientation == PHASE_VERTICAL) {
			for(int y = 0; y < height; y++) {
				memcpy(&pixels[y * width * 3], single, width * 3);
			}
		} else {
			int i = 0;
			int j = 0;
			for(int y = 0; y < height; y++) {
				for(int x = 0; x < width; x++) {
					memcpy(&pixels[i], &single[j], 3);
					i += 3;
				}
				j += 3;
			}
		}
		delete [] single;
		sequence[sequencePosition].update();
	}
}
