#include "ThreePhaseGenerator.h"

ThreePhaseGenerator::ThreePhaseGenerator() :
	orientation(PHASE_VERTICAL),
	wavelength(0),
	gamma(1) {
}

void ThreePhaseGenerator::setOrientation(phaseOrientation orientation) {
	this->orientation = orientation;
}

void ThreePhaseGenerator::setWavelength(float wavelength) {
	this->wavelength = wavelength;
}

void ThreePhaseGenerator::setGamma(float gamma) {
	this->gamma = gamma;
}

void ThreePhaseGenerator::generate() {
	allocateSequence(3);
	for(int k = 0; k < 3; k++) {
		float offset;
		switch(k) {
			case 0: offset = - TWO_PI / 3; break;
			case 1: offset = 0; break;
			case 2: offset = + TWO_PI / 3; break;
		}
		unsigned char* pixels = sequence[k].getPixels();
		float normalize = TWO_PI / wavelength;
		for(int y = 0; y < height; y++) {
			int i = 3 * y * width;
			for(int x = 0; x < width; x++) {
				float reference = orientation == PHASE_VERTICAL ? x : y;
				float curPhase = (cosf(reference * normalize + offset) + 1) / 2;
				curPhase = powf(curPhase, gamma);
				curPhase *= 256;
				if(curPhase >= 256)
					curPhase = 255;
				unsigned char value = (unsigned char) curPhase;
				pixels[i++] = value;
				pixels[i++] = value;
				pixels[i++] = value;
			}
		}
		sequence[k].update();
	}
}
