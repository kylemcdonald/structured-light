#pragma once

#include "ofMain.h"
#include "ThreadedImageCapture.h"
#include "ThreePhaseGenerator.h"

class testApp : public ofBaseApp {
public:
	testApp();
	void setup();
	void update();
	void draw();

	ThreePhaseGenerator generator;
	ThreadedImageCapture capture;
};
