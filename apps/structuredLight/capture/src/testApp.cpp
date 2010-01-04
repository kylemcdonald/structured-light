#include "testApp.h"

void testApp::frameReceived(ofVideoGrabber& grabber) {
	if(&grabber == &camera) {
		int cameraRate = panel.getValueI("cameraRate");
		int cameraOffset = panel.getValueI("cameraOffset");
		int sequenceSize = curGenerator->size();
		int curFrame = cameraFrameNum + cameraOffset;
		// the size is 0 when we're preparing it in another thread
		if(sequenceSize != 0) {
			if(curFrame % cameraRate == 0) {
				curFrame = (curFrame / cameraRate) % sequenceSize;
				int n = (int) camera.getWidth() * (int) camera.getHeight() * 3;
				if(!hidden) {
					camera.getThreadedPixels(recent[curFrame].getPixels());
					needsUpdate[curFrame] = true;
				} else {
					string filename = "capture/" + ofToString(cameraFrameNum) + ".jpg";
					imageSaver.setFilename(filename);
					camera.getThreadedPixels(imageSaver.getPixels());
					imageSaver.next();
				}
			}
			cameraFrameNum++;
		}
	}
}

void testApp::setup(){
	hidden = false;
	lastEnableCamera = false;
	cameraFrameNum = 0;

	threePhase.setSize(1024, 768);
	threePhase.setWavelength(64);
	threePhase.generate();

	grayCode.setSize(1024, 768);
	grayCode.setLength(1024);
	grayCode.setSubdivisions(10);
	grayCode.generate();

	gradient.setSize(1024, 768);
	gradient.generate();

	// setup panel
	panel.setup("control", 8, 8, 300, 700);
	panel.loadSettings("control.xml");
	panel.addPanel("capture", 1);
	panel.addPanel("three phase", 1);
	panel.addPanel("gray code", 1);

	panel.setWhichPanel("capture");
	panel.addToggle("enable camera", "enableCamera", false);
	panel.addToggle("camera settings", "cameraSettings", false);
	panel.addToggle("fullscreen", "fullscreen", false);
	vector<string> patternTypes;
	patternTypes.push_back("three phase");
	patternTypes.push_back("gray code");
	patternTypes.push_back("gradient");
	panel.addMultiToggle("pattern type", "patternType", 0, patternTypes);
	vector<string> orientations;
	orientations.push_back("vertical");
	orientations.push_back("horizonal");
	panel.addMultiToggle("orientation", "orientation", 0, orientations);
	panel.addToggle("reverse", "reverse", false);
	panel.addSlider("pattern rate", "patternRate", 1, 1, 6, true);
	panel.addSlider("camera rate", "cameraRate", 1, 1, 6, true);
	panel.addSlider("camera offset", "cameraOffset", 0, 0, 5, true);

	panel.setWhichPanel("three phase");
	panel.addSlider("wavelength", "wavelength", 64, 8, 512, true);

	panel.setWhichPanel("gray code");
	panel.addSlider("subdivisions", "subdivisions", 10, 1, 10, true);

	cameraWidth = 640;
	cameraHeight = 480;
	int n = 10; // no patterns have more than 10 frames
	for(int i = 0; i < n; i++) {
		recent.push_back(ofImage());
		recent.back().allocate(
			cameraWidth,
			cameraHeight,
			OF_IMAGE_COLOR);
		needsUpdate.push_back(false);
	}
	curGenerator = &threePhase;

	int captureTime = 15;
	imageSaver.setup(cameraWidth, cameraHeight, captureTime * 60);

	ofBackground(0, 0, 0);
}

void testApp::update(){
	// this is where an event/callback-based
	// control panel would be really helpful!
	int curWavelength = panel.getValueI("wavelength");
	if(curWavelength != lastWavelength) {
		threePhase.setWavelength(curWavelength);
		threePhase.generate();
	}
	lastWavelength = curWavelength;

	int curOrientation = panel.getValueI("orientation");
	if(curOrientation != lastOrientation) {
		phaseOrientation orientation = curOrientation == 0 ? PHASE_VERTICAL : PHASE_HORIZONTAL;
		threePhase.setOrientation(orientation);
		threePhase.generate();
		grayCode.setOrientation(orientation);
		grayCode.generate();
		gradient.setOrientation(orientation);
		gradient.generate();
	}
	lastOrientation = curOrientation;

	int curSubdivisions = panel.getValueI("subdivisions");
	if(curSubdivisions != lastSubdivisions) {
		grayCode.setSubdivisions(curSubdivisions);
		grayCode.generate();
	}
	lastSubdivisions = curSubdivisions;

	threePhase.setReverse(panel.getValueB("reverse"));
	grayCode.setReverse(panel.getValueB("reverse"));

	int curFullscreen = panel.getValueB("fullscreen");
	if(curFullscreen !=	lastFullscreen)
		ofSetFullscreen(curFullscreen);
	lastFullscreen = curFullscreen;

	bool curEnableCamera = panel.getValueB("enableCamera");
	if(curEnableCamera != lastEnableCamera) {
		if(curEnableCamera) {
			camera.setup(cameraWidth, cameraHeight, this);
		} else {
			camera.stopThread();
			camera.close();
		}
	}
	lastEnableCamera = curEnableCamera;

	if(panel.getValueB("cameraSettings")) {
		camera.videoSettings();
		panel.setValueB("cameraSettings", false);
	}

	int curPatternType = panel.getValueI("patternType");
	if(curPatternType != lastPatternType) {
		switch(curPatternType) {
			case 0: curGenerator = &threePhase; break;
			case 1: curGenerator = &grayCode; break;
			case 2: curGenerator = &gradient; break;
		}
	}
	lastPatternType = curPatternType;
}

void testApp::draw(){
	int patternFrame = ofGetFrameNum() / panel.getValueI("patternRate");
	curGenerator->get(patternFrame).draw(0, 0);
	if(!hidden) {
		int cameraRate = panel.getValueI("cameraRate");
		int cameraOffset = panel.getValueI("cameraOffset");
		ofPushStyle();
		ofPushMatrix();
		int padding = 8;
		ofTranslate(panel.getWidth() + padding * 2, padding);
		int recentWidth = 160;
		int recentHeight = 120;
		for(int i = 0; i < curGenerator->size(); i++) {
			ofPushMatrix();
			ofTranslate(
				(i % 3) * (recentWidth + padding),
				(i / 3) * (recentHeight + padding));
			ofSetColor(255, 255, 255);
			if(needsUpdate[i]) {
				recent[i].update();
				needsUpdate[i] = false;
			}
			recent[i].draw(0, 0, recentWidth, recentHeight);
			ofSetColor(255, 0, 0);
			ofNoFill();
			ofRect(0, 0, recentWidth, recentHeight);
			ofPopMatrix();
		}
		ofPopMatrix();
		ofPopStyle();
	}
}

void testApp::keyPressed(int key) {
	if(key == 'f') {
		ofToggleFullscreen();
	}
	if(key == '\t') {
		hidden = !hidden;
		panel.hidden = hidden;
		if(hidden)
			ofHideCursor();
		else
			ofShowCursor();
		if(!hidden)
			imageSaver.saveAll();
	}
}
