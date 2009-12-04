#pragma once

#include "PatternGenerator.h"
#include "PhaseConstants.h"

class ThreePhaseGenerator : public PatternGenerator {
private:
	float wavelength;
	phaseOrientation orientation;
	float gamma; // should be in PatternGenerator
public:
	ThreePhaseGenerator();
	void setOrientation(phaseOrientation orientation);
	void setWavelength(float wavelength);
	void setGamma(float gamma);
	void generate();
};
