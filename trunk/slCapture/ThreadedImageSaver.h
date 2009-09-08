#pragma once

#include "ofxThread.h"
#include "GlobalLogger.h"
#include <queue>

struct SaveableImage {
	ofImage img;
	ofImage temp;
	string filename;
	void saveImage() {
		img.saveImage(filename);
	}
	void rotate() {
		int w = (int) img.getWidth();
		int h = (int) img.getHeight();
		temp.allocate(h, w, img.type);
		unsigned char* srcPixels = img.getPixels();
		unsigned char* destPixels = temp.getPixels();
		int destPosition = 0, srcPosition = 0;
		for(int y = 0; y < h; y++) {
			for(int x = 0; x < w; x++) {
				destPosition = ((x + 1) * h - y) * 3;
				srcPosition = (y * w + x) * 3;
				destPixels[destPosition + 0] = srcPixels[srcPosition + 0];
				destPixels[destPosition + 1] = srcPixels[srcPosition + 1];
				destPixels[destPosition + 2] = srcPixels[srcPosition + 2];
			}
		}
		img.allocate(h, w, img.type);
		img.setFromPixels(destPixels, h, w, img.type);
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
			images.front().rotate();
			images.front().saveImage();
			images.pop();
		}
	}
};
