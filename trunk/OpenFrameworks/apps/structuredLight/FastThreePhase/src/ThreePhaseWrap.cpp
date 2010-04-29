#include "ThreePhaseWrap.h"

#ifdef USE_WRAP_LUT
bool ThreePhaseWrap::phaseGammaLutReady = false;
unsigned char ThreePhaseWrap::phaseGammaLut[WRAP_LUT_STEPS][WRAP_LUT_STEPS][WRAP_LUT_STEPS][2];
#endif

ThreePhaseWrap::ThreePhaseWrap() :
		threshold(16) {
	#ifdef USE_WRAP_LUT
	if(!phaseGammaLutReady) {
		cout << "Building phase wrap lookup table... ";
		float sqrt3 = sqrtf(3.);
		float ipi = 128. / M_PI;
		for(int i1 = 0; i1 < WRAP_LUT_STEPS; i1++) {
			for(int i2 = 0; i2 < WRAP_LUT_STEPS; i2++) {
				for(int i3 = 0; i3 < WRAP_LUT_STEPS; i3++) {
					int a = i1 - i3;
					int b = 2 * i2 - i1 - i3;
					float r = atan2f(sqrt3 * a, b) * ipi;
					float g = sqrtf(3 * a * a + b * b) / (i1 + i2 + i3);
					phaseGammaLut[i1][i2][i3][0] = 128 + (unsigned char) r;
					phaseGammaLut[i1][i2][i3][1] = (unsigned char) (255 * g);
				}
			}
		}
		cout << "done." << endl;
		phaseGammaLutReady = true;
	}
	#endif
}

ThreePhaseWrap::~ThreePhaseWrap() {
	delete [] phaseImage[0];
	delete [] phaseImage[1];
	delete [] phaseImage[2];
}

void ThreePhaseWrap::setup(int width, int height) {
	this->width = width;
	this->height = height;
	n = width * height;
	for(int i = 0; i < 3; i++)
		phaseImage[i] = new unsigned char[n];
}

void ThreePhaseWrap::setThreshold(unsigned char threshold) {
	this->threshold = threshold;
}

void ThreePhaseWrap::set(int phase, const unsigned char* image, int channels) {
	if(channels == 1) {
		memcpy(phaseImage[phase], image, n);
	} else if(channels == 3) {
		ImageUtility::convertRgbToGrayscale(image, phaseImage[phase], width, height);
	}
}

unsigned char* ThreePhaseWrap::get(int phase) {
	return phaseImage[phase];
}

void ThreePhaseWrap::makeWrappedPhase(unsigned char* wrappedPhase, unsigned char* mask) {
	float sqrt3 = sqrtf(3.);
	float ipi = 128. / M_PI;
	unsigned char* p1itr = phaseImage[0];
	unsigned char* p2itr = phaseImage[1];
	unsigned char* p3itr = phaseImage[2];
	for(int i = 0; i < n; i++) {
		int i1 = *(p1itr++);
		int i2 = *(p2itr++);
		int i3 = *(p3itr++);

		#ifdef USE_WRAP_LUT
		i1 >>= WRAP_LUT_ACCURACY;
		i2 >>= WRAP_LUT_ACCURACY;
		i3 >>= WRAP_LUT_ACCURACY;
		unsigned char* cur = phaseGammaLut[i1][i2][i3];
		wrappedPhase[i] = cur[0];
		mask[i] = cur[1] < threshold ? LABEL_BACKGROUND : 0;
		#else
		int a = i1 - i3;
		int b = 2 * i2 - i1 - i3;
		float r = atan2f(sqrt3 * a, b) * ipi;
		wrappedPhase[i] = 128 + (unsigned char) r;
		float g = sqrtf(3 * a * a + b * b) / (i1 + i2 + i3);
		mask[i] = (g * 255) < threshold ? LABEL_BACKGROUND : 0;
		#endif
	}
}
