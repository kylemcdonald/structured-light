#ifndef _VISION_DIY3DSCAN_SRC_ThreePhaseGenerator
#define _VISION_DIY3DSCAN_SRC_ThreePhaseGenerator

#include "PatternGenerator.h"
#include "PhaseConstants.h"

#include "libexport.h"

class DLL_EXPORT ThreePhaseGenerator : public PatternGenerator {
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

#endif
