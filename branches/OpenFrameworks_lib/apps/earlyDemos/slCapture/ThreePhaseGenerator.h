#pragma once

#include "PatternGenerator.h"

union color {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

enum threephaseMode {THREEPHASE_SINE, THREEPHASE_LINE};

class ThreePhaseGenerator : public PatternGenerator {
public:
	ThreePhaseGenerator() {
		wavelength = 128;
		gamma = 1;
		mode = THREEPHASE_SINE;
		maximum = 255;
		reverse = false;
	}
	void setWavelength(float wavelength) {
		this->wavelength = wavelength;
	}
	void setGamma(float gamma) {
		this->gamma = gamma;
	}
	void setOrientation(generatorOrientation orientation) {
		this->orientation = orientation;
	}
	void setMode(threephaseMode mode) {
		this->mode = mode;
	}
	void setMaximum(float maximum) {
		this->maximum = maximum;
	}
	void setReverse(bool reverse) {
		this->reverse = reverse;
	}
protected:
	float wavelength, gamma, maximum;
	threephaseMode mode;
	bool reverse;
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
			#ifdef REFERENCE_COLORS
			for(int y = 0; y < height; y++)
				for(int x = 0; x < 128; x++) {
					int p = (y * width + x) * 3;
					curPixels[p + 0] = i == 0 ? 255 : 0;
					curPixels[p + 1] = i == 1 ? 255 : 0;
					curPixels[p + 2] = i == 2 ? 255 : 0;
				}
			#endif
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
		float offset = TWO_PI * (float) reference / wavelength;
		if(reverse)
			phaseOffset -= offset;
		else
			phaseOffset += offset;
		if(mode == THREEPHASE_SINE) {
			float curPhase = sin(phaseOffset);
			// why doesn't powf work for negative numbers...?
			curPhase = curPhase > 0 ?
				powf(curPhase, gamma) :
				-powf(-curPhase, gamma);
			return (unsigned char) ofMap(curPhase, -1, +1, 0, maximum);
		} else if(mode == THREEPHASE_LINE) {
			float value = asinf(sinf(phaseOffset));
			value = ofMap(value, -HALF_PI, HALF_PI, 0, 1);
			value = powf(value, gamma);
			return (unsigned char) ofMap(value, 0, 1, 0, maximum);
		}
	}
};
