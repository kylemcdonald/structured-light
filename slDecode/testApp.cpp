#include "testApp.h"

void testApp::setup(){
	decoder.setup(ofGetWidth(), ofGetHeight());
	decoder.loadImages();

	FastTimer fastTimer;

	fastTimer.start();
	for(int i = 0; i < 50; i++)
		decoder.decode();
	fastTimer.stop();
	cout << (50 / fastTimer.getSeconds()) << " fps" << endl;

	zscale = 140;
	zskew = 23;

	camera.setup(this, 1024);

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
	float** wrapphase = decoder.getWrapPhase();
  int step = 2;
  glBegin(GL_POINTS);
  for (int y = step; y < height; y += step) {
    float planephase = 0.5 - (y - (height / 2)) / zskew;
    for (int x = step; x < width; x += step) {
      if (!mask[y][x]) {
        glVertex3f(x, y, (wrapphase[y][x] - planephase) * zscale);
      }
    }
  }
  glEnd();
}
