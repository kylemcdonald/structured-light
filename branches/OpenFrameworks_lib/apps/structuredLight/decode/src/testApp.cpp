#include "testApp.h"

/*
	then implement exporting of sequences ("record").
	then allow skipping to different locations in the sequence.
*/

void testApp::setup(){
	hidden = false;
	sequenceFrame = 0;
	threePhase = NULL;

	// setup control panel
	panel.setup("control", 8, 8, 300, 700);
	panel.loadSettings("control.xml");
	panel.addPanel("input", 1);
	panel.addPanel("decode", 1);
	panel.addPanel("export", 1);

	panel.setWhichPanel("input");

	inputList.listDir("input");
	panel.addFileLister("input", &inputList, 240, 500);
	panel.addSlider("camera rate", "cameraRate", 1, 1, 6, true);
	panel.addSlider("camera offset", "cameraOffset", 0, 0, 5, true);

	panel.setWhichPanel("decode");

	panel.addToggle("play sequence", "playSequence", false);
	panel.addSlider("jump to", "jumpTo", 0, 0, 100, false);

	styles.push_back("cloud");
	styles.push_back("mesh");
	panel.addMultiToggle("style", "style", 0, styles);

	vector<string> orientation;
	orientation.push_back("horizontal");
	orientation.push_back("vertical");
	panel.addMultiToggle("orientation", "orientation", 0, orientation);

	panel.addSlider("range threshold", "rangeThreshold", 40, 0, 255, true);

	panel.addSlider("depth scale", "depthScale", 130, -500, 500, false);
	panel.addSlider("depth skew", "depthSkew", -28, -50, 50, false);

	panel.addSlider("filter min", "filterMin", -1024, -1024, 1024, false);
	panel.addSlider("filter max", "filterMax", 1024, -1024, 1024, false);

	panel.setWhichPanel("export");

	exportFormats.push_back(".obj");
	exportFormats.push_back(".ply");
	exportFormats.push_back(".png");
	panel.addMultiToggle("export format", "exportFormat", 0, exportFormats);

	panel.addToggle("export", "export", false);
	panel.addToggle("record", "record", false);

	panel.addToggle("render movie", "renderMovie", false);
	panel.addSlider("movie framerate", "movieFramerate", 60, 5, 60, true);

	ofBackground(0, 0, 0);
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
	for(int y = 0; y < srcHeight; y++) {
		for(int x = 0; x < srcWidth; x++) {
			if(!mask[i]) {
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
	for(int y = 0; y < srcHeight - 1; y++) {
		for(int x = 0; x < srcWidth - 1; x++) {
			int nw = y * srcWidth + x;
			int ne = nw + 1;
			int sw = nw + srcWidth;
			int se = ne + srcWidth;
			if(!mask[nw] && !mask[se]) {
				if(!mask[ne]) { // nw, ne, se
					glColor3ubv(&color[nw * 3]);
					glVertex3f(x, y, depth[nw]);
					glColor3ubv(&color[ne * 3]);
					glVertex3f(x + 1, y, depth[ne]);
					glColor3ubv(&color[se * 3]);
					glVertex3f(x + 1, y + 1, depth[se]);
				}
				if(!mask[sw]) { // nw, se, sw
					glColor3ubv(&color[nw * 3]);
					glVertex3f(x, y, depth[nw]);
					glColor3ubv(&color[se * 3]);
					glVertex3f(x + 1, y + 1, depth[se]);
					glColor3ubv(&color[sw * 3]);
					glVertex3f(x, y + 1, depth[sw]);
				}
			} else if(!mask[ne] && !mask[sw]) {
				if(!mask[nw]) { // nw, ne, sw
					glColor3ubv(&color[nw * 3]);
					glVertex3f(x, y, depth[nw]);
					glColor3ubv(&color[ne * 3]);
					glVertex3f(x + 1, y, depth[ne]);
					glColor3ubv(&color[sw * 3]);
					glVertex3f(x, y + 1, depth[sw]);
				}
				if(!mask[se]) { // ne, se, sw
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

testApp::~testApp() {
}

void testApp::nextFrame() {
	int cameraRate = panel.getValueI("cameraRate");
	int cameraOffset = panel.getValueI("cameraOffset");
	int cameraFrame = (sequenceFrame * cameraRate) + cameraOffset;
	cameraFrame %= totalImages;
	if(usingDirectory) {
		ofImage phase;
		phase.loadImage(inputDir + imageList.getName(cameraFrame));
		threePhase->set(sequenceFrame % 3, phase.getPixels());
	} else {
		movieInput.setFrame(cameraFrame);
		threePhase->set(sequenceFrame % 3, movieInput.getPixels());
	}
	if(sequenceFrame > (totalImages / cameraRate)) {
		sequenceFrame = 0;
		cout << "Looping sequence." << endl;
	} else
		sequenceFrame++;
}

void testApp::jumpTo(unsigned frame) {
	sequenceFrame = frame;
	for(int i = 0; i < 3; i++)
		nextFrame();
}

void testApp::setupInput() {
	if(threePhase != NULL)
		delete threePhase;
	threePhase = new ThreePhaseDecoder();
	string name = inputList.getSelectedName();
	usingDirectory = name.find('.') == string::npos;
	if(usingDirectory) {
		inputDir = "input/" + name + "/";
		totalImages = imageList.listDir(inputDir);
		ofImage phase;
		phase.loadImage(inputDir + imageList.getName(0));
		threePhase->setup((int) phase.getWidth(), (int) phase.getHeight());
	} else {
		movieInput.loadMovie("input/" + name);
		movieInput.setVolume(0);
		totalImages = movieInput.getTotalNumFrames();
		threePhase->setup((int) movieInput.getWidth(), (int) movieInput.getHeight());
	}
	jumpTo(0);
}

void testApp::update(){
	float curDepthScale = panel.getValueF("depthScale");
	float curDepthSkew = panel.getValueF("depthSkew");
	int curRangeThreshold = panel.getValueI("rangeThreshold");
	int curOrientation = panel.getValueI("orientation");
	float curFilterMin = panel.getValueF("filterMin");
	float curFilterMax = panel.getValueF("filterMax");
	bool curPlaySequence = panel.getValueB("playSequence");
	float curJumpTo = panel.getValueF("jumpTo");
	int curCameraRate = panel.getValueI("cameraRate");
	int curCameraOffset = panel.getValueI("cameraOffset");

	bool reload = inputList.selectedHasChanged();
	if(reload) {
		setupInput();
		inputList.clearChangedFlag();
	}

	if(threePhase != NULL) {
		threePhase->setDepthScale(curDepthScale);
		threePhase->setDepthSkew(curDepthSkew);
		threePhase->setRangeThreshold(curRangeThreshold);
		threePhase->setOrientation(curOrientation == 0 ? PHASE_HORIZONTAL : PHASE_VERTICAL);

		unsigned targetFrame = (unsigned) ofMap(lastJumpTo, 0, 100, 0, totalImages / curCameraRate);

		if(curJumpTo != lastJumpTo) {
			jumpTo(targetFrame);
			reload = true;
		} else if(curPlaySequence || curCameraRate != lastCameraRate || curCameraOffset != lastCameraOffset) {
			nextFrame();
			panel.setValueF("jumpTo", ofMap(sequenceFrame, 0, totalImages / curCameraRate, 0, 100));
			curJumpTo = panel.getValueF("jumpTo");
			reload = true;
		}

		if(reload ||
				curRangeThreshold != lastRangeThreshold || curOrientation != lastOrientation ||
				curFilterMin != lastFilterMin || curFilterMax != lastFilterMax ||
				curDepthScale != lastDepthScale || curDepthSkew != lastDepthSkew) {
			threePhase->decode();
			if(curFilterMin != -1024 || curFilterMax != 1024)
				threePhase->filterRange(curFilterMin, curFilterMax);
		}

		// export handling
		bool curExport = panel.getValueB("export");
		bool curRecord = panel.getValueB("record");
		if(curExport || curRecord) {
			string curFormat = exportFormats[panel.getValueI("exportFormat")];
			string name = inputList.getSelectedName();
			if(curRecord)
				name += "-" + ofToString(sequenceFrame);
			if(curFormat == ".png") {
				threePhase->exportDepth("output/" + name + "-depth.png", panel.getValueI("filterMin"), panel.getValueI("filterMax"));
			} else {
				int curStyle = panel.getValueI("style");
				string outputFile = "output/" + name + "-" + styles[curStyle] + curFormat;
				if(curStyle == 0) {
					threePhase->exportCloud(outputFile);
				} else if(curStyle == 1) {
					threePhase->exportMesh(outputFile);
				}
			}
			panel.setValueB("export", false);
		}
	}

	lastDepthScale = curDepthScale;
	lastDepthSkew = curDepthSkew;
	lastRangeThreshold = curRangeThreshold;
	lastOrientation = curOrientation;
	lastFilterMin = curFilterMin;
	lastFilterMax = curFilterMax;
	lastJumpTo = curJumpTo;
	lastCameraRate = curCameraRate;
	lastCameraOffset = curCameraOffset;
}

void testApp::getBounds(ofxPoint3f& min, ofxPoint3f& max) {
	bool* mask = threePhase->getMask();
	float* depth = threePhase->getDepth();

	int srcWidth = threePhase->getWidth();
	int srcHeight = threePhase->getHeight();

	min.set(srcWidth, srcHeight, panel.getValueF("filterMax"));
	max.set(0, 0, panel.getValueF("filterMin"));

	int i = 0;
	for(int y = 0; y < srcHeight; y++) {
		for(int x = 0; x < srcWidth; x++) {
			if(!mask[i]) {
				if(x < min.x)
					min.x = x;
				if(x > max.x)
					max.x = x;
				if(y < min.y)
					min.y = y;
				if(y > max.y)
					max.y = y;
				if(depth[i] < min.z)
					min.z = depth[i];
				if(depth[i] > max.z)
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
	glVertex3s(0,0,0); glVertex3s(1,0,0);
	glVertex3s(0,0,0); glVertex3s(0,1,0);
	glVertex3s(1,1,0); glVertex3s(1,0,0);
	glVertex3s(1,1,0); glVertex3s(0,1,0);
	// front
	glVertex3s(0,0,1); glVertex3s(1,0,1);
	glVertex3s(0,0,1); glVertex3s(0,1,1);
	glVertex3s(1,1,1); glVertex3s(1,0,1);
	glVertex3s(1,1,1); glVertex3s(0,1,1);
	// extrusion
	glVertex3s(0,0,0); glVertex3s(0,0,1);
	glVertex3s(0,1,0); glVertex3s(0,1,1);
	glVertex3s(1,0,0); glVertex3s(1,0,1);
	glVertex3s(1,1,0); glVertex3s(1,1,1);
	glEnd();
	ofPopMatrix();
}

void testApp::draw(){
	camera.place();
	glEnable(GL_DEPTH_TEST);

	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);

	if(threePhase != NULL) {
		ofTranslate(-threePhase->getWidth() / 2, -threePhase->getHeight() / 2);

		if(!hidden) {
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
		bool useCloud = panel.getValueI("style") == 0;
		if(useCloud) {
			drawCloud();
		} else {
			drawMesh();
		}
	}

	glDisable(GL_DEPTH_TEST);

	camera.remove();

	if(panel.getValueB("renderMovie") && hidden) {
		screenCapture.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		movieOutput.addFrame(screenCapture.getPixels(), 1. / panel.getValueI("movieFramerate"));
	}

	glPopMatrix();

	if ((threePhase != NULL) && panel.getValueB("hud")) {

		int srcWidth = threePhase->getWidth();
		int srcHeight = threePhase->getHeight();

		if (redraw) {
			bool* mask = threePhase->getMask();
			float* depth = threePhase->getDepth();
			byte* color = threePhase->getColor();
			float* phase = threePhase->getPhase();
			float* wrappedPhase = threePhase->getWrappedPhase();
			float* range = threePhase->getRange();
			float* unwrapOrder = threePhase->unwrapOrder;

			unsigned char* upix = phaseUnwrapped.getPixels();
			unsigned char* ppix = phaseWrapped.getPixels();
			unsigned char* rpix = rangeIm.getPixels();
			unsigned char* opix = unwrapOrderIm.getPixels();
			unsigned char* dpix = depthIm.getPixels();

			///float minPhase = threePhase->minPhase;
			///float maxPhase = threePhase->maxPhase;
			float minPhase = -panel.getValueF("maxPhase");
			float maxPhase = -minPhase;

			// threePhase->minDepth,threePhase->maxDepth
			float maxDepth = pow(10, panel.getValueF("maxDepth"));
			float minDepth = -maxDepth;

			int i = 0;
			for (int y = 0; y < srcHeight; y++) {
				for (int x = 0; x < srcWidth; x++) {
					ppix[i] = (char) ofMap(wrappedPhase[i],
					                            0, 1,
					                            0, 255);

					rpix[i] = (char) range[i];

					float mag;
					ofColor col;

					if (!mask[i]) {
						mag = ofMap(phase[i], minPhase, maxPhase, 0.0, 1.0);
					} else {
						mag = 0.5;
					}
					col = makeColor(mag);

					upix[i*3] =  (char)col.r;
					upix[i*3+1] = (char)col.g;
					upix[i*3+2] = (char)col.b;

					mag = 1.0 - ofMap(depth[i], minDepth, maxDepth, 0.0, 1.0);
					col = makeColor(mag);
					dpix[i*3] = (short) col.r;
					dpix[i*3+1] = (short) col.g;
					dpix[i*3+2] = (short) col.b;

					mag = unwrapOrder[i] / (float)(srcWidth * srcHeight);
					col = makeColor(mag);
					opix[i*3] = (short) col.r;
					opix[i*3+1] = (short) col.g;
					opix[i*3+2] = (short) col.b;

					i++;
				}
			}
			phaseUnwrapped.update();
			phaseWrapped.update();
			rangeIm.update();
			unwrapOrderIm.update();
			depthIm.update();

		}

		int w = (int)panel.getValueF("hudWidth");
		float hOff = panel.getValueF("hudHeightOffset");

		ofSetColor(255, 255, 255);
		glColor3f(1, 1, 1);
		float sc = (float)srcHeight / (float)srcWidth;
		hOff *= -2 * w * sc;

		phaseUnwrapped.getTextureReference().draw(ofGetWidth() - w, hOff + 0*w*sc, w, w*sc);
		depthIm.getTextureReference().draw(ofGetWidth() - w, hOff + 1*w*sc, w, w*sc);
		rangeIm.getTextureReference().draw(ofGetWidth() - w, hOff + 2*w*sc, w, w*sc);

		unwrapOrderIm.getTextureReference().draw(ofGetWidth() - 2*w, hOff + 0*w*sc, w, w*sc);
		phaseWrapped.getTextureReference().draw(ofGetWidth() - 2*w, hOff + 1*w*sc, w, w*sc);
	}
}

void testApp::keyPressed(int key) {
	if(key == 'f')
		ofToggleFullscreen();
	if(key == '\t') {
		hidden = !hidden;
		panel.hidden = hidden;
		if(panel.getValueB("renderMovie")) {
			if(hidden) {
				screenCapture.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
				movieOutput.setup(ofGetWidth(), ofGetHeight(), "output/" + ofToString(ofGetFrameNum()) + ".mov");
			} else {
				movieOutput.finishMovie();
			}
		}
	}
}
