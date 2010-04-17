#pragma once

#include "ofMain.h"
#include "ThreePhaseDecoder.h"
#include "ofxEasyCam.h"
#include "FastTimer.h"
#include "ofxDirList.h"

#include "ofxQtVideoSaver.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void timeDecoder();
	void keyPressed(int key);

	ThreePhaseDecoder decoder;
	vector<string> files;
	ofxEasyCam camera;
	int startOffset;
	bool modifyDecoder;

	ofxQtVideoSaver saver;
	ofImage screen;
	bool recording;
};
