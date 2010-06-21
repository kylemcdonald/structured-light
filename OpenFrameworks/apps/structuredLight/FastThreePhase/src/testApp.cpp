#include "testApp.h"

void testApp::setup() {
	ofSetVerticalSync(false);

	cout << "Loading images... ";
	ofImage phase1, phase2, phase3;
	string directory = "zhang";
	string extension = "png";
	phase1.loadImage(directory + "/phase1." + extension);
	phase2.loadImage(directory + "/phase2." + extension);
	phase3.loadImage(directory + "/phase3." + extension);

	inputWidth = (int) phase1.getWidth();
	inputHeight = (int) phase2.getHeight();

	ofSetWindowShape(inputWidth, inputHeight);

	phase.allocate(inputWidth, inputHeight, OF_IMAGE_GRAYSCALE);
	quality.allocate(inputWidth, inputHeight, OF_IMAGE_GRAYSCALE);
	offset.allocate(inputWidth, inputHeight, OF_IMAGE_GRAYSCALE);
	unwrapped.allocate(inputWidth, inputHeight, OF_IMAGE_GRAYSCALE);

	phasePixels = phase.getPixels();
	qualityPixels = quality.getPixels();
	offsetPixels = offset.getPixels();
	unwrappedPixels = unwrapped.getPixels();

	cout << "Setting up decoding instances..." << endl;
	threePhaseWrap.setup(inputWidth, inputHeight);
	partialQualityMap.setup(inputWidth, inputHeight);
	scanlineOffset.setup(inputWidth, inputHeight);
	unwrap.setup(inputWidth, inputHeight);
	cout << "Done setting up decoding instances." << endl;

	cout << "Converting to Grayscale..." << endl;
	phase1.setImageType(OF_IMAGE_GRAYSCALE);
	phase2.setImageType(OF_IMAGE_GRAYSCALE);
	phase3.setImageType(OF_IMAGE_GRAYSCALE);

	cout << "sending to threePhaseWrap... ";
	int channels = 1;
	threePhaseWrap.set(0, phase1.getPixels(), channels);
	threePhaseWrap.set(1, phase2.getPixels(), channels);
	threePhaseWrap.set(2, phase3.getPixels(), channels);
	cout << "done." << endl;

	at = bt = ct = dt = 0;
}

void testApp::update(){
	int w = inputWidth;
	int h = inputHeight;
	int n = w * h;

	int wrapThreshold = mouseX;
	if(wrapThreshold > 254)
		wrapThreshold = 254;
	threePhaseWrap.setThreshold(wrapThreshold);

/*
	int scanlineThreshold = mouseY;
	if(scanlineThreshold > 254)
		scanlineThreshold = 254;
	scanlineOffset.setThreshold(scanlineThreshold);
*/

	int a = ofGetElapsedTimeMillis();
	threePhaseWrap.makeWrappedPhase(phasePixels, qualityPixels); // 1.8 ms
	int b = ofGetElapsedTimeMillis();
	partialQualityMap.makeQualityMap(phasePixels, qualityPixels); // 1.8 ms
	int c = ofGetElapsedTimeMillis();
	scanlineOffset.makeOffset(phasePixels, qualityPixels, (char*) offsetPixels); // 3.4 ms
	int d = ofGetElapsedTimeMillis();
	unwrap.makeUnwrapped(qualityPixels, phasePixels, (char*) offsetPixels, unwrappedPixels); // 1.3 ms
	int e = ofGetElapsedTimeMillis();

	at += b - a;
	bt += c - b;
	ct += d - c;
	dt += e - d;
}

void testApp::draw(){
	glColor3f(1, 1, 1);

	ofImage& show = unwrapped;
	show.update();
	show.draw(0, 0);

	if(ofGetFrameNum() == 0)
		ofSaveScreen("phase.png");

	// draw histogram
	int* qualityHistogram = scanlineOffset.getQualityHistogram();
	glColor3f(1, 1, 1);
	int maxQuality = 0;
	for(int i = 0; i < 256; i++)
		if(qualityHistogram[i] > maxQuality)
			maxQuality = qualityHistogram[i];
	glBegin(GL_LINE_STRIP);
	for(int i = 0; i < 256; i++)
		glVertex2f(i, (128 * (float) qualityHistogram[i]) / maxQuality);
	glEnd();

	// draw levels
	int* levelCutoff = scanlineOffset.getLevelCutoff();
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	for(int i = 0; i < LEVEL_COUNT; i++) {
		glVertex2i(levelCutoff[i], 0);
		glVertex2i(levelCutoff[i], 128);
	}
	glEnd();

	ofSetColor(255, 255, 255);
	int t = ofGetFrameNum() + 1;
	int us = 100;
	int a = (at * us) / t;
	int b = (bt * us) / t;
	int c = (ct * us) / t;
	int d = (dt * us) / t;
	int total = (a + b + c + d) + 1;
	int fps = (1000 * us) / total;
	ofDrawBitmapString(
		"wrap " + ofToString(a) + "us + " +
		"quality " + ofToString(b) + "us + " +
		"offset " + ofToString(c) + "us + " +
		"unwrap " + ofToString(d) + "us = " +
		ofToString(total) + "us / " + ofToString(fps) + " fps",
		10, ofGetHeight() - 10);
}
