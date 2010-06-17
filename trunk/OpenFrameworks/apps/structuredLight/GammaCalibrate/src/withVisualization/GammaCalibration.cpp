#include "GammaCalibration.h"

GammaCalibration::GammaCalibration() :
		sampledLocations(128),
		smoothness(4) {
}

void GammaCalibration::setSampledLocations(int setSampledLocations) {
	this->sampledLocations = sampledLocations;
}

void GammaCalibration::setSmoothness(float smoothness) {
	this->smoothness = smoothness;
}

void GammaCalibration::calibrate(vector<ofImage> images, vector<float> exposureTimes) {
	for(int i = 0; i < exposureTimes.size(); i++)
		exposureTimes[i] = logf(exposureTimes[i]) / logf(M_E);

	int sampledExposures = images.size();

	CvMat *A, *x, *b;

	CvMat* inputCurves = cvCreateMat(sampledLocations, sampledExposures, CV_8UC1);
	CvMat* inputExposures = cvCreateMat(sampledExposures, 1, CV_32FC1);
	CvMat* weighting = cvCreateMat(levels, 1, CV_8UC1);

	unsigned char* inputCurvesPtr = (unsigned char*) cvPtr2D(inputCurves, 0, 0);
	float* inputExposuresPtr = (float*) cvPtr1D(inputExposures, 0);
	unsigned char* weightingPtr = (unsigned char*) cvPtr1D(weighting, 0);
}
