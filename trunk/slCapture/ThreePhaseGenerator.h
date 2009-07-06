#pragma once

#include "PatternGenerator.h"

union color {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

class ThreePhaseGenerator : public PatternGenerator {
public:
	ThreePhaseGenerator() {
		wavelength = 128;
	}
	void setWavelength(float wavelength) {
		this->wavelength = wavelength;
	}
	void setOrientation(generatorOrientation orientation) {
		this->orientation = orientation;
	}
protected:
	float wavelength;
	generatorOrientation orientation;
	void _generate() {
		for(int i = 0; i < 3; i++) {
			ofImage curImage;
			curImage.allocate(width, height, OF_IMAGE_COLOR);
			unsigned char* curPixels = curImage.getPixels();
			for(int y = 0; y < height; y++)
				for(int x = 0; x < width; x++) {
					unsigned char cur = makeValue(i, x, y, orientation);
					int p = (y * width + x) * 3;
					curPixels[p + 0] = cur;
					curPixels[p + 1] = cur;
					curPixels[p + 2] = cur;
				}
			for(int y = 0; y < height; y++)
				for(int x = 0; x < 128; x++) {
					int p = (y * width + x) * 3;
					curPixels[p + 0] = i == 0 ? 255 : 0;
					curPixels[p + 1] = i == 1 ? 255 : 0;
					curPixels[p + 2] = i == 2 ? 255 : 0;
				}
			sequence.push_back(curImage);
		}
	}
	unsigned char makeValue(int stage, int x, int y, generatorOrientation orientation) {
		int reference = 0;
		if(orientation == GENERATOR_HORIZONTAL)
			reference = y;
	  else if(orientation == GENERATOR_VERTICAL)
			reference = x;
		float phaseOffset = (float) stage * TWO_PI / 3.;
		float curOffset = TWO_PI * (float) reference / wavelength;
		float curPhase = sin(phaseOffset + curOffset);
		// 255 on following line should technically be 256
		// so everything is spaced evenly...
		return (unsigned char) ofMap(curPhase, -1, +1, 0, 255);
	}
};
