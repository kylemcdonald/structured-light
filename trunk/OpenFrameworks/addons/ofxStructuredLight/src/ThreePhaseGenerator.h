#pragma once

#include "PatternGenerator.h"
#include "PhaseConstants.h"

class ThreePhaseGenerator : public PatternGenerator {
private:
	float wavelength;
	phaseOrientation orientation;
public:
    int minBrightness;

	ThreePhaseGenerator();
	void setOrientation(phaseOrientation orientation);
	void setWavelength(float wavelength);
	void generate();
};
