#ifndef _VISION_DIY3DSCAN_SRC_PhaseDecoder
#define _VISION_DIY3DSCAN_SRC_PhaseDecoder

#include "DepthDecoder.h"
#include "FastBlur.h"
#include "PhaseConstants.h"

#include <string>

#include "libexport.h"

class DLL_EXPORT PhaseDecoder : public DepthDecoder {
public:
	PhaseDecoder();
	virtual ~PhaseDecoder();
	void setDepthScale(float depthScale);
	float getDepthScale();
	void setDepthSkew(float depthSkew);
	float getDepthSkew();
	void setOrientation(phaseOrientation orientation);
	void setMaxPasses(int maxPasses);
	void setMinRemaining(float minRemaining);
	void setPhasePersistence(bool phasePersistence);
	void set(int i, byte* image);
	void makeDepth();
	void decode();
	float* getPhase();
	byte* getColor();
	int* getBlur();
	void exportCloud(std::string filename);
	void exportMesh(std::string filename);
protected:
	int sequenceSize;
	byte** colorSequence;
	byte** graySequence;
	float* phase;
	bool* ready;
	float depthScale, depthSkew;
	int maxPasses;
	float minRemaining;
	byte* color;
	phaseOrientation orientation;
	FastBlur blur;
	bool phasePersistence;
	float* lastPhase;

	void setup(int width, int height, int sequenceSize);
	virtual void makePhase() = 0;
	virtual void unwrapPhase() = 0;
	virtual void makeColor() = 0;
	virtual int getStart();
	float getRemaining();
};

#endif
