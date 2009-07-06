#pragma once

#include "ofMain.h"
#include "ThreadedImageCapture.h"
#include "ThreePhaseGenerator.h"
#include "GlobalLogger.h"

class testApp : public ofBaseApp {
public:
	testApp();
	void setup();
	void update();
	void draw();

	ThreePhaseGenerator generator;
	ThreadedImageCapture capture;
	ofTrueTypeFont font;
};
