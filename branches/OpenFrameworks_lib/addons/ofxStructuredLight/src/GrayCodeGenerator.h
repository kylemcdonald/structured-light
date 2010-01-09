#pragma once

#include "PatternGenerator.h"
#include "PhaseConstants.h"

#include "libexport.h"

class DLL_EXPORT GrayCodeGenerator : public PatternGenerator {
private:
	unsigned int length, subdivisions;
	phaseOrientation orientation;
public:
	GrayCodeGenerator();
	void setOrientation(phaseOrientation orientation);
	void setLength(unsigned int length);
	void setSubdivisions(unsigned int subdivisions);
	void generate();
};
