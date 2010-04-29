#pragma once

#include "ofMain.h"

#include "ThreePhaseWrap.h"
#include "PartialQualityMap.h"
#include "ScanlineOffset.h"
#include "Unwrap.h"

const int
	inputWidth = 518,
	inputHeight = 388;

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
};
