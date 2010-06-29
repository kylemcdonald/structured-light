#include "GradientGenerator.h"

GradientGenerator::GradientGenerator() :
	orientation(PHASE_VERTICAL) {
}

void GradientGenerator::setOrientation(phaseOrientation orientation) {
	this->orientation = orientation;
}

void GradientGenerator::generate() {
	allocateSequence(8);
	for(int k = 0; k < 8; k++) {
		int q = k % 4;
		int length = orientation == PHASE_HORIZONTAL ? height : width;
		unsigned char* single = new unsigned char[length * 3];
		int i = 0;
		for(unsigned int j = 0; j < length; j++) {
#ifdef OPENFRAMEWORKS_AVAIL
			unsigned char value = (unsigned char) ofClamp((256 * j) / length, 0, 255);
#else
			unsigned char value = 0;//TODO
#endif
			if(k != q)
				value = 255 - value;
			single[i++] = q == 0 || q == 1 ? value : 0;
			single[i++] = q == 0 || q == 2 ? value : 0;
			single[i++] = q == 0 || q == 3 ? value : 0;
		}
#ifdef OPENFRAMEWORKS_AVAIL
		unsigned char* pixels = sequence[k].getPixels();
#else
		unsigned char* pixels = (unsigned char*)(sequence[k].imageData);
#endif
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
