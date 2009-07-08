#pragma once

#include "ofMain.h"
#include "ThreePhaseDecoder.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	ThreePhaseDecoder decoder;

	float zscale, zskew;
};
