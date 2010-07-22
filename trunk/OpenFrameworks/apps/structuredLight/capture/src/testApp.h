#pragma once

#include "ofMain.h"
#include "ofxStructuredLight.h"
#include "ofxAutoControlPanel.h"
#include "ofxThreadedVideoGrabber.h"
#include "ofxImageSaver.h"


class testApp : public ofBaseApp{

public:
	void frameReceived(ofVideoGrabber& grabber);

	void setup();
	void update();
	void draw();
	void keyPressed(int key);

	bool hidden;
	ofxAutoControlPanel panel;

	ThreePhaseGenerator threePhase;
	int lastWavelength;

	GrayCodeGenerator grayCode;
	int lastSubdivisions;

	TwoPlusOneGenerator twoPlusOne;

	GradientGenerator gradient;

	PatternGenerator* curGenerator;
	int lastPatternType;
	int lastOrientation;
	bool lastFullscreen;

	int lastMinBrightness;

	ofxThreadedVideoGrabber camera;
	ofxImageSaver imageSaver;

	int cameraWidth, cameraHeight;
	ofImage cameraImage;
	vector<ofImage> recent;
	vector<bool> needsUpdate;
	bool lastEnableCamera;
	int lastCameraRate;
	int cameraFrameNum;
	int patternFrame;

	bool blackout;
};
