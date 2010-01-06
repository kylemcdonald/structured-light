#pragma once

#include "PatternGenerator.h"
#include "PhaseConstants.h"

class RgbThreePhaseGenerator : public PatternGenerator {
private:
	float wavelength;
	phaseOrientation orientation;
public:
	RgbThreePhaseGenerator();
	void setOrientation(phaseOrientation orientation);
	void setWavelength(float wavelength);
	void generate();
};
