#pragma once

#include "ofMain.h"
#include "ofxThread.h"
#include "ThreadedImageSaver.h"
#include "GlobalLogger.h"

class ThreadedImageCapture : ofxThread {
public:
	void setup(int width, int height) {
		this->width = width;
		this->height = height;
		frameNumber = 0;
		frameOffset = 0;
		loopbackDelay = 3;
		grabber.setVerbose(true);
		grabber.initGrabber(width, height, false);
		curImage.allocate(width, height, OF_IMAGE_COLOR);
		startSeconds = 0;
		startThread(false, false);
	}
	int getEffectiveFrameNumber() {
		return frameNumber / loopbackDelay;
	}
	int getFrameNumber() {
		return frameNumber;
	}
	float getFrameRate() {
		return (float) frameNumber / (ofGetElapsedTimef() - startSeconds);
	}
	int getSaveSize() {
		return saver.size();
	}
protected :
	int frameNumber, frameOffset, loopbackDelay;
	float startSeconds;
	int width, height;
	ofVideoGrabber grabber;
	ofImage curImage;
	ThreadedImageSaver saver;

	void threadedFunction() {
		while(true) {
			grabber.grabFrame();
			if(grabber.isFrameNew()) {
				frameNumber++;
				if(startSeconds == 0)
					startSeconds = ofGetElapsedTimef();
				curImage.setFromPixels(grabber.getPixels(), width, height, OF_IMAGE_COLOR);
				if((frameNumber + frameOffset) % loopbackDelay == 0) {
					stringstream filename;
					filename << getEffectiveFrameNumber();
					filename << ".jpg";
					saver.save(curImage, filename.str());
				}
			}
		}
	}
};
