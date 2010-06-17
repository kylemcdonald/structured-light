#pragma once

#include "ofxOpenCv.h"

class GammaCalibration {
public:
	GammaCalibration();
	void setSampledLocations(int sampledLocations);
	void setSmoothness(float smoothness);
	void calibrate(vector<ofImage> images, vector<float> exposureTimes);
private:
	static const int levels = 256;
	int sampledLocations;
	float smoothness;
};
