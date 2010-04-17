#include "testApp.h"

void testApp::frameReceived(ofVideoGrabber& cur) {
	if(&cur == &grabber) {
		grabber.getThreadedPixels(recent[curFrame].getPixels());
		threePhase->set(curFrame, recent[curFrame].getPixels());
		needsDecode = true;
		needsUpdate[curFrame] = true;
		curFrame = (curFrame + 1) % 3;
	}
}

void testApp::setup() {
	hidden = false;
	threePhase = NULL;

	cameraWidth = 640;
	cameraHeight = 480;
	curFrame = 0;

	for(int i = 0; i < 3; i++){
		recent[i].allocate(cameraWidth, cameraHeight, OF_IMAGE_COLOR);
		needsUpdate[i] = false;
	}
	needsDecode = false;

	// setup control panel
	panel.setup("control", 0, 0, 300, 720);
	panel.loadSettings("control.xml");

	panel.addPanel("camera", 1);
	panel.setWhichPanel("camera");

	panel.addToggle("start camera", "startCamera", false);
	panel.addSlider("gamma", "gamma", 1, 0, 1, false);

	panel.addPanel("decode", 1);
	panel.setWhichPanel("decode");

	panel.addToggle("phase persistence", "phasePersistence", false);
	panel.addToggle("reset view ", "resetView", false);

	styles.push_back("cloud");
	styles.push_back("mesh");
	styles.push_back("none");
	panel.addMultiToggle("style", "style", 0, styles);

	vector<string> orientation;
	orientation.push_back("horizontal");
	orientation.push_back("vertical");
	panel.addMultiToggle("orientation", "orientation", 0, orientation);

	panel.addSlider("range threshold", "rangeThreshold", 40, 0, 255, true);

	panel.addSlider("depth scale", "depthScale", 130, -200, 200, false);
	panel.addSlider("depth skew", "depthSkew", -28, -150, 150, false);

	panel.addSlider("filter min", "filterMin", -1024, -1024, 1024, false);
	panel.addSlider("filter max", "filterMax", 1024, -1024, 1024, false);
}

void testApp::drawCloud() {
	bool* mask = threePhase->getMask();
	float* depth = threePhase->getDepth();
	byte* color = threePhase->getColor();

	int srcWidth = threePhase->getWidth();
	int srcHeight = threePhase->getHeight();

	glEnable(GL_POINT_SIZE);
	glPointSize(3);
	glBegin(GL_POINTS);
	int i = 0;
	for (int y = 0; y < srcHeight; y++) {
		for (int x = 0; x < srcWidth; x++) {
			if (!mask[i]) {
				glColor3ubv(&color[i * 3]);
				glVertex3f(x, y, depth[i]);
			}
			i++;
		}
	}
	glEnd();
}


void testApp::drawMesh() {
	bool* mask = threePhase->getMask();
	float* depth = threePhase->getDepth();
	byte* color = threePhase->getColor();

	int srcWidth = threePhase->getWidth();
	int srcHeight = threePhase->getHeight();

	glBegin(GL_TRIANGLES);
	for (int y = 0; y < srcHeight - 1; y++) {
		for (int x = 0; x < srcWidth - 1; x++) {
			int nw = y * srcWidth + x;
			int ne = nw + 1;
			int sw = nw + srcWidth;
			int se = ne + srcWidth;
			if (!mask[nw] && !mask[se]) {
				if (!mask[ne]) { // nw, ne, se
					glColor3ubv(&color[nw * 3]);
					glVertex3f(x, y, depth[nw]);
					glColor3ubv(&color[ne * 3]);
					glVertex3f(x + 1, y, depth[ne]);
					glColor3ubv(&color[se * 3]);
					glVertex3f(x + 1, y + 1, depth[se]);
				}
				if (!mask[sw]) { // nw, se, sw
					glColor3ubv(&color[nw * 3]);
					glVertex3f(x, y, depth[nw]);
					glColor3ubv(&color[se * 3]);
					glVertex3f(x + 1, y + 1, depth[se]);
					glColor3ubv(&color[sw * 3]);
					glVertex3f(x, y + 1, depth[sw]);
				}
			} else if (!mask[ne] && !mask[sw]) {
				if (!mask[nw]) { // nw, ne, sw
					glColor3ubv(&color[nw * 3]);
					glVertex3f(x, y, depth[nw]);
					glColor3ubv(&color[ne * 3]);
					glVertex3f(x + 1, y, depth[ne]);
					glColor3ubv(&color[sw * 3]);
					glVertex3f(x, y + 1, depth[sw]);
				}
				if (!mask[se]) { // ne, se, sw
					glColor3ubv(&color[ne * 3]);
					glVertex3f(x + 1, y, depth[ne]);
					glColor3ubv(&color[se * 3]);
					glVertex3f(x + 1, y + 1, depth[se]);
					glColor3ubv(&color[sw * 3]);
					glVertex3f(x, y + 1, depth[sw]);
				}
			}
		}
	}
	glEnd();
}

void testApp::update() {
	float curDepthScale = panel.getValueF("depthScale");
	float curDepthSkew = panel.getValueF("depthSkew");
	int curRangeThreshold = panel.getValueI("rangeThreshold");
	int curOrientation = panel.getValueI("orientation");
	float curFilterMin = panel.getValueF("filterMin");
	float curFilterMax = panel.getValueF("filterMax");
	bool curPhasePersistence = panel.getValueB("phasePersistence");
	float gamma = panel.getValueF("gamma");

	bool curResetView = panel.getValueB("resetView");
	if (curResetView) {
		camera = ofxEasyCam();
		panel.setValueB("resetView", false);
	}

	bool startCamera = panel.getValueB("startCamera");
	if(startCamera) {
		if (threePhase != NULL)
			delete threePhase;
		threePhase = new ThreePhaseDecoder();
		threePhase->setup(cameraWidth, cameraHeight);
		grabber.setup(cameraWidth, cameraHeight, this, false);
		panel.setValueB("startCamera", false);
	}

	bool reload = false;

	if (threePhase != NULL) {
		threePhase->setGamma(gamma);
		threePhase->setDepthScale(curDepthScale);
		threePhase->setDepthSkew(curDepthSkew);
		threePhase->setRangeThreshold(curRangeThreshold);
		threePhase->setOrientation(curOrientation == 0 ? PHASE_HORIZONTAL : PHASE_VERTICAL);
		threePhase->setPhasePersistence(curPhasePersistence);

		if (curPhasePersistence != lastPhasePersistence)
			threePhase->clearLastPhase();
	}

	lastGamma = gamma;
	lastDepthScale = curDepthScale;
	lastDepthSkew = curDepthSkew;
	lastRangeThreshold = curRangeThreshold;
	lastOrientation = curOrientation;
	lastFilterMin = curFilterMin;
	lastFilterMax = curFilterMax;
	lastPhasePersistence = curPhasePersistence;
}

void testApp::getBounds(ofxPoint3f& min, ofxPoint3f& max) {
	bool* mask = threePhase->getMask();
	float* depth = threePhase->getDepth();

	int srcWidth = threePhase->getWidth();
	int srcHeight = threePhase->getHeight();

	min.set(srcWidth, srcHeight, panel.getValueF("filterMax"));
	max.set(0, 0, panel.getValueF("filterMin"));

	int i = 0;
	for (int y = 0; y < srcHeight; y++) {
		for (int x = 0; x < srcWidth; x++) {
			if (!mask[i]) {
				if (x < min.x)
					min.x = x;
				if (x > max.x)
					max.x = x;
				if (y < min.y)
					min.y = y;
				if (y > max.y)
					max.y = y;
				if (depth[i] < min.z)
					min.z = depth[i];
				if (depth[i] > max.z)
					max.z = depth[i];
			}
			i++;
		}
	}
}

void testApp::boxOutline(ofxPoint3f min, ofxPoint3f max) {
	ofPushMatrix();
	ofTranslate(min.x, min.y, min.z);
	ofScale(max.x - min.x, max.y - min.y, max.z - min.z);
	glBegin(GL_LINES);
	// back
	glVertex3s(0, 0, 0);
	glVertex3s(1, 0, 0);
	glVertex3s(0, 0, 0);
	glVertex3s(0, 1, 0);
	glVertex3s(1, 1, 0);
	glVertex3s(1, 0, 0);
	glVertex3s(1, 1, 0);
	glVertex3s(0, 1, 0);
	// front
	glVertex3s(0, 0, 1);
	glVertex3s(1, 0, 1);
	glVertex3s(0, 0, 1);
	glVertex3s(0, 1, 1);
	glVertex3s(1, 1, 1);
	glVertex3s(1, 0, 1);
	glVertex3s(1, 1, 1);
	glVertex3s(0, 1, 1);
	// extrusion
	glVertex3s(0, 0, 0);
	glVertex3s(0, 0, 1);
	glVertex3s(0, 1, 0);
	glVertex3s(0, 1, 1);
	glVertex3s(1, 0, 0);
	glVertex3s(1, 0, 1);
	glVertex3s(1, 1, 0);
	glVertex3s(1, 1, 1);
	glEnd();
	ofPopMatrix();
}

void testApp::drawAxes(float size) {
	ofPushMatrix();
	ofScale(size, size, size);
	ofPushStyle();
	ofSetLineWidth(2.0);
	ofSetColor(255, 0, 0);
	glBegin(GL_LINES);
	glVertex3s(0, 0, 0);
	glVertex3s(1, 0, 0);
	glEnd();
	ofSetColor(0, 255, 0);
	glBegin(GL_LINES);
	glVertex3s(0, 0, 0);
	glVertex3s(0, 1, 0);
	glEnd();
	ofSetColor(0, 0, 255);
	glBegin(GL_LINES);
	glVertex3s(0, 0, 0);
	glVertex3s(0, 0, 1);
	glEnd();
	ofPopStyle();
	ofPopMatrix();
}

void testApp::draw() {
	if (hidden)
		ofBackground(0, 0, 0);
	else
		ofBackground(128, 128, 128);

	glPushMatrix();
	camera.place();
	glEnable(GL_DEPTH_TEST);

	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);

	if (!hidden)
		drawAxes(256); // major axes

	if (threePhase != NULL) {
		if(needsDecode)
			threePhase->decode();

		ofTranslate(-threePhase->getWidth() / 2, -threePhase->getHeight() / 2);

		if (!hidden) {
			ofxPoint3f min, max;
			getBounds(min, max);
			ofSetColor(255, 255, 255);
			boxOutline(min, max);
			min.z = panel.getValueF("filterMin");
			max.z = panel.getValueF("filterMax");
			min -= 4;
			max += 4;
			ofSetColor(0, 0, 255);
			boxOutline(min, max);
		}

		ofSetColor(255, 255, 255);
		int useCloud = panel.getValueI("style");
		if (useCloud == 0) {
			drawCloud();
		} else if (useCloud == 1) {
			drawMesh();
		}
	}

	glDisable(GL_DEPTH_TEST);

	camera.remove();

	if (!hidden) {
		ofSetColor(255, 255, 255);

		for(int i = 0; i < 3; i++) {
			if(needsUpdate[i]) {
				recent[i].update();
				needsUpdate[i] = false;
			}
			int resizeWidth = cameraWidth / 3;
			int resizeHeight = cameraHeight / 3;
			recent[i].draw(
				ofGetWidth() - resizeWidth, resizeHeight * i + i,
				resizeWidth, resizeHeight);
		}
	}

	glPopMatrix();
}

void testApp::keyPressed(int key) {
	if (key == 's')
		grabber.videoSettings();
	if (key == 'f')
		ofToggleFullscreen();
	if (key == '\t') {
		hidden = !hidden;
		panel.hidden = hidden;
	}
}
