#pragma once

#include "ofMain.h"
#include "ofxThread.h"
#include "ThreadedImageSaver.h"

class ThreadedImageCapture : ofxThread {
public:
	void setup(int width, int height) {
		this->width = width;
		this->height = height;
		frameNumber = 0;
		grabber.setVerbose(true);
		grabber.initGrabber(width, height, false);
		curImage.allocate(width, height, OF_IMAGE_COLOR);
		startThread();
		startSeconds = 0;
		grabber.listDevices();
	}
	int getFrameNumber() {
		return frameNumber;
	}
	float getFrameRate() {
		return (float) frameNumber / (ofGetElapsedTimef() - startSeconds);
	}
protected :
	int frameNumber;
	float startSeconds;
	int width, height;
	ofVideoGrabber grabber;
	ofImage curImage;
	ThreadedImageSaver saver;

	void threadedFunction() {
		while(true) {
			grabber.grabFrame();
			if(grabber.isFrameNew()) {
				if(startSeconds == 0)
					startSeconds = ofGetElapsedTimef();
				curImage.setFromPixels(grabber.getPixels(), width, height, OF_IMAGE_COLOR);
				stringstream filename;
				filename << frameNumber++;
				filename << ".jpg";
				saver.save(curImage, filename.str());
				cout << getFrameRate() << endl;
			}
			ofSleepMillis(1000 / 200); // 200 fps update resolution
		}
	}
};
