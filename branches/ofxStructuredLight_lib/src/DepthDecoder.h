#ifndef _VISION_DIY3DSCAN_SRC_DepthDecoder
#define _VISION_DIY3DSCAN_SRC_DepthDecoder

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
	virtual void set(int i, byte* image) = 0;
	virtual void decode() = 0;
	bool* getMask();
	float* getDepth();
	void filterDeviations(float deviations);
	void filterRange(float min, float max);
	void exportDepth(std::string filename);
	virtual void exportCloud(std::string filename);
	virtual void exportMesh(std::string filename);
protected:
	int width, height;
	bool* mask;
	float* depth;
};

#endif
