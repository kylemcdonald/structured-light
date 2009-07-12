#include "testApp.h"

void testApp::setup(){
	decoder.setup(ofGetWidth(), ofGetHeight());
	decoder.decode();

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

	bool** _process = decoder.getProcess();
	float** _wrapphase = decoder.getWrapPhase();
  int step = 2;
  glBegin(GL_POINTS);
  for (int y = step; y < height; y += step) {
    float planephase = 0.5 - (y - (height / 2)) / zskew;
    for (int x = step; x < width; x += step) {
      if (!_process[y][x] && !_process[y - step][x - step]) {
        glVertex3f(x, y, (_wrapphase[y][x] - planephase) * zscale);
      }
    }
  }
  glEnd();
}
