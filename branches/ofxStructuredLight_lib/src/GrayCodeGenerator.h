#pragma once

#include "PatternGenerator.h"
#include "PhaseConstants.h"

class GrayCodeGenerator : public PatternGenerator {
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
