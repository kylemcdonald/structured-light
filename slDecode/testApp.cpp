#include "testApp.h"

void testApp::timeDecoder() {
	FastTimer fastTimer;
	decoder.loadImages(
		files[0],
		files[1],
		files[2]);
	float itr = 50;
	fastTimer.start();
	for(int i = 0; i < itr; i++)
		decoder.decode();
	fastTimer.stop();
	float fps = (itr / fastTimer.getSeconds());
	cout << fps << " fps / " << (1000 / fps) << " ms" << endl;
}

void testApp::setup() {
	decoder.setup(480, 640, 23.8, 172.5, 0.15);
	decoder.wide = false;
	startOffset = 0;
	modifyDecoder = false;

	ofxDirList dir;
	int n = dir.listDir(".");
	cout << n << " images to play." << endl;
	for(int i = 0; i < n; i++) {
		cout << "adding " << dir.getName(i) << endl;
		files.push_back(dir.getName(i));
	}
	std::sort(files.begin(), files.end());

	//timeDecoder();

	camera.setup(this, 512);

	recording = false;

	ofBackground(0, 0, 0);
	ofSetColor(255, 255, 255);
}

void testApp::update() {
	int position = ofGetFrameNum();
	int size = files.size();
	decoder.loadImages(
		files[(position + 0) % size],
		files[(position + 1) % size],
		files[(position + 2) % size]);
	decoder.decode((position + startOffset) % 3);
}

void testApp::draw() {
	if(modifyDecoder) {
		decoder.zskew = ofMap(mouseY, 0, ofGetHeight(), 10, 60);
		decoder.zscale = ofMap(mouseX, 0, ofGetWidth(), 10, 250);
	}

  camera.draw();

	int width = decoder.getWidth();
	int height = decoder.getHeight();

	bool** mask = decoder.getMask();
	float** depth = decoder.getDepth();
	unsigned char** color = decoder.getColor();
  int step = 1;

  glBegin(GL_POINTS);
  for (int y = step; y < height; y += step)
    for (int x = step; x < width; x += step)
      if (!mask[y][x]) {
      	ofSetColor(color[y][x * 3], color[y][x * 3 + 1], color[y][x * 3 + 2]);
        glVertex3f(x, y, depth[y][x]);
      }
  glEnd();

	if(recording) {
		screen.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		saver.addFrame(screen.getPixels(), 1. / 30.);
	}
}

void testApp::keyPressed(int key) {
	if(key == 's') {
		recording = !recording;
		if(recording) {
			saver.setCodecQualityLevel(OF_QT_SAVER_CODEC_QUALITY_NORMAL);
			saver.setup(ofGetWidth(), ofGetHeight(), "../output.mov");
			screen.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
		} else {
			saver.finishMovie();
		}
	} else if (key == ' ') {
		modifyDecoder = !modifyDecoder;
		cout << "zskew: " << decoder.zskew << endl;
		cout << "zscale: " << decoder.zscale << endl;
	} else if(key == OF_KEY_RIGHT) {
		startOffset++;
	} else if(key == OF_KEY_LEFT) {
		startOffset--;
	} else if(key == 'p') {
		ofSaveFrame();
	}
}
