#pragma once

#include "ofMain.h"
#include "ThreePhaseDecoder.h"
#include "ofxEasyCam.h"
#include "FastTimer.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void timeDecoder();

	ThreePhaseDecoder decoder;
	vector<string> files;
	ofxEasyCam camera;
};
