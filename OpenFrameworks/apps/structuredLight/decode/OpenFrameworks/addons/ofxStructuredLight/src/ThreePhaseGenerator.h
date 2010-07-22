#pragma once

#include "PatternGenerator.h"
#include "PhaseConstants.h"

class ThreePhaseGenerator : public PatternGenerator {
private:
	float wavelength;
	phaseOrientation orientation;
	float minBrightness;
public:

	void setMinBrightness(float minBrightness);

	ThreePhaseGenerator();
	void setOrientation(phaseOrientation orientation);
	void setWavelength(float wavelength);
	void generate();
};
