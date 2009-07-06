#include "testApp.h"
#include "stdio.h"

testApp::testApp(){
}

/*
	1 compute mesh in post
	2 compute mesh in real time
*/

void testApp::setup(){
	ofSetBackgroundAuto(false);
	font.loadFont("../trebuchet.ttf", 256, false, false);
	ofSetVerticalSync(true);
	generator.setWavelength(32);
	generator.setOrientation(GENERATOR_VERTICAL);
	generator.generate(ofGetWidth(), ofGetHeight());
	capture.setup(640, 480);
}

void testApp::update(){
}

void testApp::draw(){
	int cur = capture.getEffectiveFrameNumber();

	ofSetColor(255, 255, 255);
	generator.get(cur % generator.size()).draw(0, 0);
}
