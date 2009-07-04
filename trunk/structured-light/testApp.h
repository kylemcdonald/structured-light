#pragma once

#include "ofMain.h"
#include "ThreadedImageCapture.h"

class testApp : public ofBaseApp {
public:
	testApp();
	void setup();
	void update();
	void draw();

	ThreadedImageCapture capture;
};
