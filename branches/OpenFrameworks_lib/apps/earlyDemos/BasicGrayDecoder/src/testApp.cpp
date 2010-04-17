#include "testApp.h"
#include "stdio.h"

void testApp::setup(){
	grayCoded.allocate(cameraWidth, cameraHeight, OF_IMAGE_GRAYSCALE);
	binaryCoded.allocate(cameraWidth, cameraHeight, OF_IMAGE_GRAYSCALE);
	thresholded.allocate(cameraWidth, cameraHeight, OF_IMAGE_GRAYSCALE);
	grayDecoder.setup(10);
	loadCurImage();
}

void testApp::loadCurImage() {
	curImage.allocate(cameraWidth, cameraHeight, OF_IMAGE_COLOR);
	curImage.loadImage(ofToString(grayDecoder.getCurLevel()) + ".jpg");
	curImage.setImageType(OF_IMAGE_GRAYSCALE);
	grayDecoder.addImage(curImage);
	grayDecoder.getGrayCodedDisplacement(grayCoded);
	grayDecoder.getBinaryCodedDisplacement(binaryCoded);
	grayDecoder.getThresholded(thresholded);
}

void testApp::update(){
}

void testApp::draw(){
	curImage.draw(0, 0, ofGetWidth() / 2, ofGetHeight() / 2);
	thresholded.draw(ofGetWidth() / 2, 0, ofGetWidth() / 2, ofGetHeight() / 2);
	grayCoded.draw(0, ofGetHeight() / 2, ofGetWidth() / 2, ofGetHeight() / 2);
	binaryCoded.draw(ofGetWidth() / 2, ofGetHeight() / 2, ofGetWidth() / 2, ofGetHeight() / 2);
}

void testApp::keyPressed(int key) {
	loadCurImage();
}
