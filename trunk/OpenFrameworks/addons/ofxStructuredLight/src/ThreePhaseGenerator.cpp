#include "ThreePhaseGenerator.h"

ThreePhaseGenerator::ThreePhaseGenerator() :
	orientation(PHASE_VERTICAL),
	wavelength(0) {
}

void ThreePhaseGenerator::setOrientation(phaseOrientation orientation) {
	this->orientation = orientation;
}

void ThreePhaseGenerator::setWavelength(float wavelength) {
	this->wavelength = wavelength;
}

void ThreePhaseGenerator::setMinBrightness(float minBrightness) {
    this->minBrightness = minBrightness;
}

void ThreePhaseGenerator::generate() {
	allocateSequence(3);
	float offsets[] = {-TWO_PI / 3, 0, +TWO_PI / 3};
	for(int k = 0; k < 3; k++) {
		unsigned char* pixels = sequence[k].getPixels();
		float normalize = TWO_PI / wavelength;
		int side = orientation == PHASE_VERTICAL ? width : height;
		unsigned char* single = new unsigned char[side * 3];
		int i = 0;
		for(int j = 0; j < side; j++) {
			float curPhase = (cosf(j * normalize + offsets[k]) + 1) / 2;
			curPhase *= (256-minBrightness);
			curPhase += minBrightness;
			if(curPhase >= 256)
				curPhase = 255;
			unsigned char value = (unsigned char) curPhase;
			single[i++] = value;
			single[i++] = value;
			single[i++] = value;
		}
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
		sequence[k].update();
	}
}
