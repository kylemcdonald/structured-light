#include "testApp.h"
#include "stdio.h"

testApp::testApp(){
}

void testApp::setup(){
	ofSetVerticalSync(true);
	ofSetBackgroundAuto(false);
	generator.setWavelength(48);
	generator.setOrientation(GENERATOR_VERTICAL);
	generator.setReverse(true);
	generator.setMode(THREEPHASE_LINE);
	generator.setMaximum(254);
	generator.setGamma(1);
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
