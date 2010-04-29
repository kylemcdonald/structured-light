#pragma once

#include "ofMain.h"

#include "ThreePhaseWrap.h"
#include "PartialQualityMap.h"
#include "ScanlineOffset.h"
#include "Unwrap.h"

const int
	inputWidth = 640,
	inputHeight = 480;

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	ThreePhaseWrap threePhaseWrap;
	PartialQualityMap partialQualityMap;
	ScanlineOffset scanlineOffset;
	Unwrap unwrap;

	ofImage phase, quality, offset, unwrapped;
	unsigned char *qualityPixels, *offsetPixels, *phasePixels, *unwrappedPixels;

	int at, bt, ct, dt;
};
