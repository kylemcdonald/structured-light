#pragma once

#include "PatternGenerator.h"
#include "PhaseConstants.h"

#include "libexport.h"

class DLL_EXPORT GradientGenerator : public PatternGenerator {
private:
	phaseOrientation orientation;
public:
	GradientGenerator();
	void setOrientation(phaseOrientation orientation);
	void generate();
};
