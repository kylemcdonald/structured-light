#include "testApp.h"

void testApp::setup(){
	decoder.setup(ofGetWidth(), ofGetHeight(), 23, 140);
	decoder.loadImages("phase1.jpg", "phase2.jpg", "phase3.jpg");

	FastTimer fastTimer;

	float itr = 50;
	fastTimer.start();
	for(int i = 0; i < itr; i++)
		decoder.decode();
	fastTimer.stop();
	float fps = (itr / fastTimer.getSeconds());
	cout << fps << " fps / " << (1000 / fps) << " ms" << endl;

	camera.setup(this, 512);

	ofBackground(0, 0, 0);
	ofSetColor(255, 255, 255);
}

void testApp::update(){
}

void testApp::draw(){
  camera.draw();

	int width = decoder.getWidth();
	int height = decoder.getHeight();

	bool** mask = decoder.getMask();
	float** depth = decoder.getDepth();
  int step = 2;
  glBegin(GL_POINTS);
  for (int y = step; y < height; y += step)
    for (int x = step; x < width; x += step)
      if (!mask[y][x])
        glVertex3f(x, y, depth[y][x]);
  glEnd();
}
