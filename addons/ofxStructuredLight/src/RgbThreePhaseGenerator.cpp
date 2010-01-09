#include "RgbThreePhaseGenerator.h"

#include "basics.h"

RgbThreePhaseGenerator::RgbThreePhaseGenerator() :
	PatternGenerator(),
	wavelength(0),
	orientation(PHASE_VERTICAL) {
}

void RgbThreePhaseGenerator::setOrientation(phaseOrientation orientation) {
	this->orientation = orientation;
}

void RgbThreePhaseGenerator::setWavelength(float wavelength) {
	this->wavelength = wavelength;
}

#ifdef OPENFRAMEWORKS_AVAIL
#define makeRgbPhase(k, j, normalize, rgb) \
	(unsigned char) ofClamp(256 * (cosf(j * normalize + offsets[rgb][k]) + 1) / 2, 0, 255)
#else
//TODO
#define makeRgbPhase(k, j, normalize, rgb) \
	0
#endif
void RgbThreePhaseGenerator::generate() {
	allocateSequence(3);
	float offsets[][3] = {
		{0, +TWO_PI / 3, -TWO_PI / 3},
		{-TWO_PI / 3, 0, +TWO_PI / 3},
		{+TWO_PI / 3, -TWO_PI / 3, 0}
	};
	for(int k = 0; k < 3; k++) {
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
			single[i++] = makeRgbPhase(k, j, normalize, 0);
			single[i++] = makeRgbPhase(k, j, normalize, 1);
			single[i++] = makeRgbPhase(k, j, normalize, 2);
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
