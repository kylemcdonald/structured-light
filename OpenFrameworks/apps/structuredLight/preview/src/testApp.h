#pragma once

#include "ofMain.h"
#include "ofxEasyCam.h"
#include "ofxStructuredLight.h"
#include "ofxAutoControlPanel.h"
#include "ofxQtVideoSaver.h"
#include "ofxThreadedVideoGrabber.h"

#include <fstream>

class testApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);

	void frameReceived(ofVideoGrabber& grabber);

	void drawCloud();
	void drawMesh();

	void getBounds(ofxPoint3f& min, ofxPoint3f& max);
	void boxOutline(ofxPoint3f min, ofxPoint3f max);
	void drawAxes(float size);

	ofxEasyCam camera;

	int cameraWidth, cameraHeight;
	int curFrame;
	ofxThreadedVideoGrabber grabber;
	ofImage recent[3];
	bool needsUpdate[3];
	bool needsDecode;

	ThreePhaseDecoder* threePhase;

	ofxAutoControlPanel panel;
	vector<string> styles;
	bool hidden;

	float lastDepthScale, lastDepthSkew;
	float lastFilterMin, lastFilterMax;
	int lastRangeThreshold, lastOrientation;
	float lastGamma;
	bool lastPhasePersistence;
};
