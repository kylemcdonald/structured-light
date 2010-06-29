#pragma once

#include "DepthExporter.h"

#include <string>

#ifndef OPENFRAMEWORKS_AVAIL
#include <cv.h>
#endif

typedef unsigned char byte;

#include "libexport.h"

class DLL_EXPORT DepthDecoder {
public:
	DepthDecoder();
	virtual ~DepthDecoder();
	int getWidth();
	int getHeight();
	virtual void set(int i, byte* image, int channels) = 0;
	virtual void decode() = 0;
	bool* getMask();
	float* getDepth();
	void filterDeviations(float deviations);
	void filterRange(float min, float max);
	void exportDepth(string filename, float min, float max);
	virtual void exportCloud(string filename);
	virtual void exportMesh(string filename);
	float avgDepth;
	float minDepth;
	float maxDepth;
protected:
	int width, height;
	bool* mask;
	float* depth;
};
