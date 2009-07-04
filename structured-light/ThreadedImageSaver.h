#pragma once

#include "ofxThread.h"
#include <queue>

class ThreadedImageSaver : public ofxThread {
public:
	void save(const ofImage& img, const string& filename) {
		lock();
		images.push(img);
		filenames.push(filename);
		if(!isThreadRunning())
			startThread(false, false);
		unlock();
	}
protected:
	queue<string> filenames;
	queue<ofImage> images;
	void threadedFunction() {
		while(images.size() > 0) {
			images.front().saveImage(filenames.front());
			images.pop();
			filenames.pop();
		}
	}
};
