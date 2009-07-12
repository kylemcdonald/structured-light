#pragma once

#include "ofMain.h"
#include "ThreePhaseDecoder.h"
#include "ofxEasyCam.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	ThreePhaseDecoder decoder;
	float zscale, zskew;

	ofxEasyCam camera;
};
