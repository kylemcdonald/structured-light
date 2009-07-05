#include "testApp.h"
#include "stdio.h"

testApp::testApp(){
}

/*
 next:
	check sync with camera
	compute mesh in post
	compute mesh in real time
*/

void testApp::setup(){
	ofSetVerticalSync(true);
	generator.setWavelength(32);
	generator.setOrientation(GENERATOR_VERTICAL);
	generator.generate(ofGetWidth(), ofGetHeight());
	capture.setup(640, 480);
}

void testApp::update(){
}

void testApp::draw(){
	// this can be optimized by only drawing
	// when there is a change (isFrameNew())
	generator.get(capture.getFrameNumber() % generator.size()).draw(0, 0);
}
