#pragma once

#include "DepthDecoder.h"
#include "FastBlur.h"
#include "PhaseConstants.h"

class PhaseDecoder : public DepthDecoder {
public:
	PhaseDecoder();
	virtual ~PhaseDecoder();
	void setDepthScale(float depthScale);
	void setDepthSkew(float depthSkew);
	void setOrientation(phaseOrientation orientation);
	void setMaxPasses(int maxPasses);
	void setMinRemaining(float minRemaining);
	void setPhasePersistence(bool phasePersistence);
	void set(int i, byte* image);
	void decode();
	float* getPhase();
	byte* getColor();
	int* getBlur();
	void exportCloud(string filename);
	void exportMesh(string filename);
protected:
	byte** colorSequence;
	byte** graySequence;
	float* phase;
	bool* ready;
	byte* color;
	int sequenceSize;
	int maxPasses;
	float minRemaining;
	float depthScale, depthSkew;
	phaseOrientation orientation;
	FastBlur blur;
	bool phasePersistence;
	float* lastPhase;

	void setup(int width, int height, int sequenceSize);
	virtual void makePhase() = 0;
	virtual void unwrapPhase() = 0;
	void makeDepth();
	virtual void makeColor() = 0;
	virtual int getStart();
	float getRemaining();
};
