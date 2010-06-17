#pragma once

#include "ofMain.h"
#include "GammaCalibration.h"
#include <fstream>

class testApp : public ofBaseApp {
public:
	void setup();
	~testApp();
	void update();
	void draw();
	void mousePressed(int, int, int);
	void mouseReleased(int, int, int);

	vector<ofImage> images;

	int levels, sampledLocations, sampledExposures;
	CvMat *inputCurves, *inputExposures, *weighting;
	CvMat *A, *x, *b;

	bool ofMousePressed;

	GammaCalibration gammaCalibration;
};
