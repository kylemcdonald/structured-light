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
	decoder.setup(ofGetWidth(), ofGetHeight(), 24, 140);

	files.push_back("phase1.jpg");
	files.push_back("phase2.jpg");
	files.push_back("phase3.jpg");

	timeDecoder();

	camera.setup(this, 512);

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
	decoder.decode((position + 1) % 3);
}

void testApp::draw() {
  camera.draw();

	int width = decoder.getWidth();
	int height = decoder.getHeight();

	bool** mask = decoder.getMask();
	float** depth = decoder.getDepth();
	unsigned char** color = decoder.getColor();
  int step = 2;
  glBegin(GL_POINTS);
  for (int y = step; y < height; y += step)
    for (int x = step; x < width; x += step)
      if (!mask[y][x]) {
      	ofSetColor(color[y][x * 3], color[y][x * 3 + 1], color[y][x * 3 + 2]);
        glVertex3f(x, y, depth[y][x]);
      }
  glEnd();
}
