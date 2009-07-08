#include "testApp.h"

void testApp::setup(){
	decoder.setup(ofGetWidth(), ofGetHeight());
	decoder.decode();

	ofBackground(0, 0, 0);
	ofSetColor(255, 255, 255);

	zscale = 140;
	zskew = 23;
}

void testApp::update(){
}

void testApp::draw(){
	int width = decoder.getWidth();
	int height = decoder.getHeight();

	float distance = ofMap(mouseY, 0, height, 0, 500);
	float theta = ofMap(mouseX, 0, width, -PI, PI);
	gluLookAt(
		distance * sin(theta), 0, distance * cos(theta),
		0, 0, 0,
		0, 1, 0);

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
