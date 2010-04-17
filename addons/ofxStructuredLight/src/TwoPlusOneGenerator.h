#pragma once

#include "PatternGenerator.h"
#include "PhaseConstants.h"

class TwoPlusOneGenerator : public PatternGenerator {
private:
	phaseOrientation orientation;
	float wavelength;
	float minBrightness;
public:

	void setMinBrightness(float minBrightness);

	TwoPlusOneGenerator();
	void setOrientation(phaseOrientation orientation);
	void setWavelength(float wavelength);
	void generate();
};
