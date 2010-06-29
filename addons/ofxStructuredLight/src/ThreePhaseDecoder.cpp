#include "ThreePhaseDecoder.h"

ThreePhaseDecoder::ThreePhaseDecoder() :
	brightness(.8),
	rangeThreshold(20) {
	#ifdef LINEARIZE_PHASE
	linearize = false;
	#endif
}

ThreePhaseDecoder::~ThreePhaseDecoder() {
	if(unwrapOrder != NULL)
		delete [] unwrapOrder;
}

void ThreePhaseDecoder::setup(int width, int height) {
	PhaseDecoder::setup(width, height, 3);
	range = new float[width * height];
	unwrapOrder = new float[width * height];
}

#ifdef USE_GAMMA
float ThreePhaseDecoder::Gamma(float x, float gamma) {
	if(gamma == 1)
		return x;

	if (gamma != oldGamma) {
		for (int i = 0; i < LUT_SIZE; i++) {
			float x = (float)i / (float)LUT_SIZE;
			if (x < 0.5) {
				gammaLut[i] =  0.5 * pow(2 * x , gamma);
			} else {
				/// could cut the size of the lut in half  with a little more work
				gammaLut[i] = 1.0 - 0.5 * pow(2 * (1 - x), gamma);
			}
		}
	}
	oldGamma = gamma;

	if (x < 0) return gammaLut[0];
	if (int(x*LUT_SIZE) >= LUT_SIZE) return gammaLut[LUT_SIZE-1];

	return gammaLut[int(x*LUT_SIZE)];
}
#endif

void ThreePhaseDecoder::makePhase() {
	int n = width * height;
	float i1, i2, i3;
	for (int i = 0; i < n; i++) {
		i1 = (float) graySequence[0][i];
		i2 = (float) graySequence[1][i];
		i3 = (float) graySequence[2][i];

		#ifdef USE_GAMMA
		i1 = Gamma(i1 / 255.0, gamma) * 255.0;
		i2 = Gamma(i2 / 255.0, gamma) * 255.0;
		i3 = Gamma(i3 / 255.0, gamma) * 255.0;
		#endif

		range[i] = findRange(i1, i2, i3);
		mask[i] = range[i] <= rangeThreshold;
		ready[i] = !mask[i];

		reflectivity[i] = (byte) ((i1 + i2 + i3) / 3);

		if(ready[i])
			phase[i] = atan2f(sqrtf(3) * (i1 - i3), 2.f * i2 - i1 - i3) / (float) TWO_PI;
	}
	#ifdef LINEARIZE_PHASE
	if(linearize) {
		buildLut();
		applyLut();
	}
	#endif
}

#ifdef LINEARIZE_PHASE

void ThreePhaseDecoder::setLinearize(bool linearize) {
	this->linearize = linearize;
}

void ThreePhaseDecoder::buildLut() {
	int n = width * height;

	// clear lut
	memset(lut, 0, lutWidth * lutHeight * sizeof(float));

	// build histogram
	for(int i = 0; i < n; i++) {
		if(ready[i]) {
			// get binned reflectivity
			int binnedReflectivity = (lutHeight * reflectivity[i]) / 256;

			// get binned phase
			int binnedPhase = (int) ((phase[i] + .5) * lutWidth);
			if(binnedPhase == lutWidth)
				binnedPhase = lutWidth - 1;

			// add one to cur reflectivity and phase
			lut[binnedReflectivity][binnedPhase]++;
		}
	}

	// histogram into linearity
	for(int i = 0; i < lutHeight; i++) {
		for(int j = 1; j < lutWidth; j++) {
			lut[i][j] += lut[i][j - 1];
		}
	}

	// linearity into lut
	for(int i = 0; i < lutHeight; i++) {
		float top = lut[i][lutWidth - 1];
		if(top > 0) {
			for(int j = 0; j < lutWidth; j++)
				lut[i][j] = (lut[i][j] / top) - .5;
		}
	}

	// super naive blur
	int r = 4;
	float blurred[lutWidth];
	for(int i = 0; i < lutHeight; i++) {
		memset(blurred, 0, lutWidth * sizeof(float));
		for(int j = 0; j < lutWidth - r; j++)
			for(int k = 0; k < r; k++)
				blurred[j] += lut[i][j + k]; // add neighbors
		for(int j = 0; j < lutWidth - r; j++)
			lut[i][j] = blurred[j] / r; // copy normalized
		for(int j = lutWidth - r; j < lutWidth; j++)
			blurred[j] = lut[i][j]; // copy direct
	}
}

void ThreePhaseDecoder::applyLut() {
	int n = width * height;
	for(int i = 0; i < n; i++) {
		if(ready[i]) {
			// get binned reflectivity
			float curReflectivity = (lutHeight * reflectivity[i]) / 256;
			int binnedReflectivity = (int) curReflectivity;

			// get binned phase
			int binnedPhase = (int) ((phase[i] + .5) * lutWidth);
			if(binnedPhase == lutWidth)
				binnedPhase = lutWidth - 1;

			phase[i] = lut[binnedReflectivity][binnedPhase];
		}
	}
}

int ThreePhaseDecoder::getLutHeight() const {
	return lutHeight;
}

int ThreePhaseDecoder::getLutWidth() const {
	return lutWidth;
}

const float* ThreePhaseDecoder::getLut(int i) const {
	return lut[i];
}

#endif

void ThreePhaseDecoder::makeColor() {
	int n = width * height * 3;
	#ifdef ACCURATE_COLOR
	float i1, i2, i3, a, b, c;
	#endif
	for (int i = 0; i < n; i++) {
		#ifdef ACCURATE_COLOR
		// This comes from "Recent progresses on real-time 3D shape measurement..."
		// The scaling factor "brightness" is due to i1,i2,i3 not necessarily being
		// perfectly sampled from out of phase cosines.
		i1 = (float) colorSequence[0][i];
		i2 = (float) colorSequence[1][i];
		i3 = (float) colorSequence[2][i];
		a = i1 - i3;
		b = 2 * i2 - i1 - i3;
		c = (i1 + i2 + i3 + sqrtf(3 * a * a + b * b)) / 3;
		color[i] = (byte) (c * brightness);
		#else
		color[i] = (byte)
			(((short) colorSequence[0][i] +
			(short) colorSequence[1][i] +
			(short) colorSequence[2][i]) / 3);
		#endif
	}
}

float* ThreePhaseDecoder::getRange() {
	return range;
}

void ThreePhaseDecoder::setRangeThreshold(float rangeThreshold) {
	this->rangeThreshold = rangeThreshold;
}

void ThreePhaseDecoder::setBrightness(float brightness) {
	this->brightness = brightness;
}

void ThreePhaseDecoder::unwrapPhase() {
	FloodFillDecoder::unwrapPhase();
}
