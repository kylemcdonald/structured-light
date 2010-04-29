#include "testApp.h"

void testApp::setup() {
	ofSetVerticalSync(false);

	phase.allocate(inputWidth, inputHeight, OF_IMAGE_GRAYSCALE);
	quality.allocate(inputWidth, inputHeight, OF_IMAGE_GRAYSCALE);
	offset.allocate(inputWidth, inputHeight, OF_IMAGE_GRAYSCALE);
	unwrapped.allocate(inputWidth, inputHeight, OF_IMAGE_GRAYSCALE);

	phasePixels = phase.getPixels();
	qualityPixels = quality.getPixels();
	offsetPixels = offset.getPixels();
	unwrappedPixels = unwrapped.getPixels();

	threePhaseWrap.setup(inputWidth, inputHeight);
	partialQualityMap.setup(inputWidth, inputHeight);
	scanlineOffset.setup(inputWidth, inputHeight);
	unwrap.setup(inputWidth, inputHeight);

	ofImage phase1, phase2, phase3;
	phase1.loadImage("phase1.png");
	phase2.loadImage("phase2.png");
	phase3.loadImage("phase3.png");

	threePhaseWrap.set(0, phase1.getPixels(), 3);
	threePhaseWrap.set(1, phase2.getPixels(), 3);
	threePhaseWrap.set(2, phase3.getPixels(), 3);
}

void testApp::update(){
	int w = inputWidth;
	int h = inputHeight;
	int n = w * h;

	int wrapThreshold = mouseX;
	if(wrapThreshold > 254)
		wrapThreshold = 254;
	threePhaseWrap.setThreshold(wrapThreshold);

	int scanlineThreshold = mouseY;
	if(scanlineThreshold > 254)
		scanlineThreshold = 254;
	scanlineOffset.setThreshold(scanlineThreshold);

	threePhaseWrap.makeWrappedPhase(phasePixels, qualityPixels); // 1.8 ms
	partialQualityMap.makeQualityMap(phasePixels, qualityPixels); // 1.8 ms
	scanlineOffset.makeOffset(phasePixels, qualityPixels, (char*) offsetPixels); // 3.4 ms
	unwrap.makeUnwrapped(qualityPixels, phasePixels, (char*) offsetPixels, unwrappedPixels); // 1.3 ms
}

void testApp::draw(){
	glColor3f(1, 1, 1);

	phase.update();
	phase.draw(0, 0);
	unwrapped.update();
	unwrapped.draw(0, inputHeight);

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

	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), ofGetWidth() - 32, ofGetHeight() - 8);
}
