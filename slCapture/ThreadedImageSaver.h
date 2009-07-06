#pragma once

#include "ofxThread.h"
#include "GlobalLogger.h"
#include <queue>

struct SaveableImage {
	ofImage img;
	string filename;
	void saveImage() {
		img.saveImage(filename);
	}
};

class ThreadedImageSaver : public ofxThread {
public:
	void save(const ofImage& img, const string& filename) {
		SaveableImage cur;
		cur.img = img;
		cur.filename = filename;
		images.push(cur);
		if(!isThreadRunning())
			startThread(false, false);
	}
	int size() {
		return images.size();
	}
protected:
	queue<SaveableImage> images;
	void threadedFunction() {
		while(images.size() > 0) {
			images.front().saveImage();
			images.pop();
		}
	}
};
