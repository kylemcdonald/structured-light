#include "ThreePhaseWrap.h"

bool ThreePhaseWrap::phaseGammaLutReady = false;
unsigned char ThreePhaseWrap::phaseGammaLut[WRAP_V_SIZE][WRAP_U_SIZE][2];

ThreePhaseWrap::ThreePhaseWrap() :
		threshold(64) {
	if(!phaseGammaLutReady) {
		cout << "Building phase wrap lookup table... ";
		float sqrt3 = sqrtf(3.);
		float ipi = 128. / M_PI;
		for(int vo = 0; vo < WRAP_V_SIZE; vo++) {
			for(int uo = 0; uo < WRAP_U_SIZE; uo++) {
				int v = vo - WRAP_V_OFFSET;
				int u = uo - WRAP_U_OFFSET;
				float phase = atan2f(sqrt3 * v, u) * ipi;
				float modulation = sqrtf(3 * v * v + u * u);
				//float average = i1 + i2 + i3;
				//float gamma = modulation / average;
				phaseGammaLut[vo][uo][0] = 128 + (unsigned char) phase;
				//phaseGammaLut[vo][uo][1] = (gamma * 255) > 255 ? 255 : (unsigned char) (gamma * 255);
				phaseGammaLut[vo][uo][1] = modulation > 255 ? 255 : (unsigned char) modulation;
			}
		}
		cout << "done." << endl;
		phaseGammaLutReady = true;
	}
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

		int v = i1 - i3;
		int u = 2 * i2 - i1 - i3;

		unsigned char* cur = phaseGammaLut[v + WRAP_V_OFFSET][u + WRAP_U_OFFSET];
		wrappedPhase[i] = cur[0];
		mask[i] = cur[1] < threshold ? LABEL_BACKGROUND : 0;
	}
}
