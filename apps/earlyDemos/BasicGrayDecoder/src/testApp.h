#pragma once

#include "ofMain.h"
#include "GrayDecoder.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);

	void loadCurImage();

	static const int cameraWidth = 1600;
	static const int cameraHeight = 1200;

	ofImage curImage, grayCoded, binaryCoded, thresholded;
	GrayDecoder grayDecoder;
};
