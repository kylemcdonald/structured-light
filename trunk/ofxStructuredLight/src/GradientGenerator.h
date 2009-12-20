#pragma once

#include "PatternGenerator.h"
#include "PhaseConstants.h"

class GradientGenerator : public PatternGenerator {
private:
	phaseOrientation orientation;
public:
	GradientGenerator();
	void setOrientation(phaseOrientation orientation);
	void generate();
};
