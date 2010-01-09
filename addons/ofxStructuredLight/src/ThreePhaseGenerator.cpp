#include "ThreePhaseGenerator.h"

#include "basics.h"

ThreePhaseGenerator::ThreePhaseGenerator() :
	wavelength(0),
	orientation(PHASE_VERTICAL),
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
#ifdef OPENFRAMEWORKS_AVAIL
		unsigned char* pixels = sequence[k].getPixels();
#else
		unsigned char* pixels = (unsigned char*)(sequence[k].imageData);
#endif
		float normalize = TWO_PI / wavelength;
		int side = orientation == PHASE_VERTICAL ? width : height;
		unsigned char* single = new unsigned char[side * 3];
		int i = 0;
		for(int j = 0; j < side; j++) {
			float curPhase = (cosf(j * normalize + offset) + 1) / 2;
			curPhase = powf(curPhase, gamma);
			curPhase *= 256;
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
#ifdef OPENFRAMEWORKS_AVAIL
		sequence[k].update();
#endif
	}
}
