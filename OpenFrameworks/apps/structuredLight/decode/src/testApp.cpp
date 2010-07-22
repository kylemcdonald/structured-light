#include "testApp.h"

/*
	then implement exporting of sequences ("record").
	then allow skipping to different locations in the sequence.
*/

void testApp::setup() {
	hidden = false;
	sequenceFrame = 0;
	threePhase = NULL;

	// setup control panel
	panel.setup("control", 0, 0, 300, 720);
	panel.loadSettings("control.xml");
	panel.addPanel("input", 1);
	panel.addPanel("decode", 1);
	panel.addPanel("export", 1);
	panel.addPanel("misc", 1);

	panel.setWhichPanel("input");

	inputList.listDir("input");

	panel.addFileLister("input", &inputList, 240, 440);
	panel.addSlider("camera rate", "cameraRate", 1, 1, 6, true);
	panel.addSlider("camera offset", "cameraOffset", 0, 0, 5, true);
	panel.addSlider("play rate", "playRate", 1, 1, 60, true);

	panel.setWhichPanel("decode");

	panel.addToggle("stop motion", "stopMotion", false);
	panel.addToggle("play sequence", "playSequence", false);
	panel.addSlider("jump to", "jumpTo", 0, 0, 100, false);
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

	panel.setWhichPanel("misc");
	panel.addSlider("gamma", "gamma", 1, 0.0, 1.0, false);
	panel.addToggle("hud", "hud", false);
	panel.addSlider("hudWidth", "hudWidth", 300.0, 0.0, 2000.0, false);
	panel.addSlider("hudHeightOffset", "hudHeightOffset", 0.0, 0.0, 1.0, false);

	panel.addSlider("maxPhase", "maxPhase", 10.0, 0.0, 100.0, false);
	panel.addSlider("maxDepth power", "maxDepth", 3.0, 0.0, 5.0, false);

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

testApp::~testApp() {
}

void testApp::nextFrame() {
	int cameraRate = panel.getValueI("cameraRate");
	int cameraOffset = panel.getValueI("cameraOffset");

	unsigned totalFrames = (totalImages - cameraOffset) / cameraRate;

	int cameraFrame = (sequenceFrame * cameraRate) + cameraOffset;
	cameraFrame %= totalImages;
	if (usingDirectory) {
		ofImage phase;
		if (!phase.loadImage(inputDir + imageList.getName(cameraFrame))) {
			cout << "couldn't load file " << (inputDir + imageList.getName(cameraFrame)) << endl;
			return;
		}
		threePhase->set(sequenceFrame % 3, phase.getPixels());

	} else {
		movieInput.setFrame(cameraFrame);
		threePhase->set(sequenceFrame % 3, movieInput.getPixels()); // TBD can movies have different than 24 bpp?
	}

	sequenceFrame = (sequenceFrame + 1) % totalFrames;

	if (sequenceFrame == 0)
		cout << "Starting sequence over." << endl;
}

void testApp::jumpTo(unsigned frame) {
	sequenceFrame = frame;
	for (int i = 0; i < 3; i++)
		nextFrame();
}

void testApp::setupInput() {
	if (threePhase != NULL)
		delete threePhase;
	threePhase = new ThreePhaseDecoder();
	string name = inputList.getSelectedName();
	usingDirectory = name.find('.') == string::npos;
	if (usingDirectory) {
		inputDir = "input/" + name + "/";
		totalImages = imageList.listDir(inputDir);

		for(int i = 0; i < totalImages; i++) {
			cout << i << ": " << imageList.getName(i) << endl;
		}

		ofImage phase;
		//  TBD
		string imageName = imageList.getName(0);
		if (!phase.loadImage(inputDir + imageName)) {
			cout << "couldn't load file " << (inputDir + imageName) << endl;
			delete threePhase;
			return;
		}
		int w = (int) phase.getWidth();
		int h = (int) phase.getHeight();
		threePhase->setup(w, h);

		phaseUnwrapped.clear();
		phaseUnwrapped.allocate(w, h, OF_IMAGE_COLOR);
		phaseWrapped.clear();
		phaseWrapped.allocate(w, h, OF_IMAGE_GRAYSCALE);
		rangeIm.clear();
		rangeIm.allocate(w, h, OF_IMAGE_GRAYSCALE);
		unwrapOrderIm.clear();
		unwrapOrderIm.allocate(w, h, OF_IMAGE_COLOR);
		depthIm.clear();
		depthIm.allocate(w, h, OF_IMAGE_COLOR);

	} else {
		movieInput.loadMovie("input/" + name);
		movieInput.setVolume(0);
		totalImages = movieInput.getTotalNumFrames();
		threePhase->setup((int) movieInput.getWidth(), (int) movieInput.getHeight());
	}
	jumpTo(0);
}

void testApp::update() {

	float curDepthScale = panel.getValueF("depthScale");
	float curDepthSkew = panel.getValueF("depthSkew");
	int curRangeThreshold = panel.getValueI("rangeThreshold");
	int curOrientation = panel.getValueI("orientation");
	float curFilterMin = panel.getValueF("filterMin");
	float curFilterMax = panel.getValueF("filterMax");
	bool curPlaySequence = panel.getValueB("playSequence");
	int curPlayRate = panel.getValueI("playRate");
	float curJumpTo = panel.getValueF("jumpTo");
	bool curPhasePersistence = panel.getValueB("phasePersistence");
	int curCameraRate = panel.getValueI("cameraRate");
	int curCameraOffset = panel.getValueI("cameraOffset");
	bool curStopMotion = panel.getValueB("stopMotion");

	float gamma = panel.getValueF("gamma");

	bool curResetView = panel.getValueB("resetView");
	if (curResetView) {
		camera = ofxEasyCam();
		panel.setValueB("resetView", false);
	}

	bool reload = inputList.selectedHasChanged();
	if (reload) {
		setupInput();
		inputList.clearChangedFlag();
	}

	unsigned totalFrames = (totalImages - curCameraOffset) / curCameraRate;

	if (threePhase != NULL) {
		threePhase->setGamma(gamma);
		threePhase->setDepthScale(curDepthScale);
		threePhase->setDepthSkew(curDepthSkew);
		threePhase->setRangeThreshold(curRangeThreshold);
		threePhase->setOrientation(curOrientation == 0 ? PHASE_HORIZONTAL : PHASE_VERTICAL);
		threePhase->setPhasePersistence(curPhasePersistence);

		if (curPhasePersistence != lastPhasePersistence)
			threePhase->clearLastPhase();

		if (curJumpTo != lastJumpTo) {
			// map slider to entire range of input images
			unsigned targetFrame = (unsigned) ofMap(lastJumpTo, 0, 100, 0, totalFrames);
			// clamp to amaxmimum of last image - 3, so you don't try reading in a loop
			targetFrame = (unsigned) ofClamp(targetFrame, 0, totalFrames - 3);
			// quantize location if stop motion is enabled
			if (curStopMotion)
				targetFrame = (targetFrame / 3) * 3;
			// so long as we're not just jumping to the same place
			if ((targetFrame + 3) % totalFrames != sequenceFrame) {
				jumpTo(targetFrame);
				reload = true;
			}
		} else if (ofGetFrameNum() % curPlayRate == 0 &&
		           (curPlaySequence || curCameraRate != lastCameraRate || curCameraOffset != lastCameraOffset)) {
			if (curStopMotion) {
				// make sure to quantize current frame, in case we're switching from non-stop-motion
				sequenceFrame = (sequenceFrame / 3) * 3;
				for (int i = 0; i < 3; i++)
					nextFrame();
			} else
				nextFrame();
			panel.setValueF("jumpTo", ofMap(sequenceFrame, 0, totalFrames, 0, 100));
			curJumpTo = panel.getValueF("jumpTo");
			reload = true;
		}

		if (reload ||
		    gamma != lastGamma ||
		    curRangeThreshold != lastRangeThreshold || curOrientation != lastOrientation ||
		    curFilterMin != lastFilterMin || curFilterMax != lastFilterMax ||
		    curDepthScale != lastDepthScale || curDepthSkew != lastDepthSkew) {
			threePhase->decode();
			if (curFilterMin != -1024 || curFilterMax != 1024)
				threePhase->filterRange(curFilterMin, curFilterMax);

			redraw = true;
		}

		// export handling
		bool curExport = panel.getValueB("export");
		bool curRecord = panel.getValueB("record");
		if (curExport || curRecord) {
			string curFormat = exportFormats[panel.getValueI("exportFormat")];
			string name = inputList.getSelectedName();
			if (curRecord)
				name += "-" + ofToString(sequenceFrame);
			if (curFormat == ".png") {
				threePhase->exportDepth("output/" + name + "-depth.png", panel.getValueI("filterMin"), panel.getValueI("filterMax"));
				threePhase->exportTexture("output/" + name + "-texture.png");
			} else {
				int curStyle = panel.getValueI("style");
				string outputFile = "output/" + name + "-" + styles[curStyle] + curFormat;
				if (curStyle == 0) {
					threePhase->exportCloud(outputFile);
				} else if (curStyle == 1) {
					threePhase->exportMesh(outputFile);
				}
			}
			panel.setValueB("export", false);
		}
	}

	lastGamma = gamma;
	lastDepthScale = curDepthScale;
	lastDepthSkew = curDepthSkew;
	lastRangeThreshold = curRangeThreshold;
	lastOrientation = curOrientation;
	lastFilterMin = curFilterMin;
	lastFilterMax = curFilterMax;
	lastJumpTo = curJumpTo;
	lastCameraRate = curCameraRate;
	lastCameraOffset = curCameraOffset;
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

	if (threePhase != NULL) {
		if (!hidden)
			drawAxes(256); // major axes

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

	if (panel.getValueB("renderMovie") && hidden) {
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
	if (key == 'f')
		ofToggleFullscreen();
	if (key == '\t') {
		hidden = !hidden;
		panel.hidden = hidden;
		if (panel.getValueB("renderMovie")) {
			if (hidden) {
				screenCapture.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
				movieOutput.setup(ofGetWidth(), ofGetHeight(), "output/" + ofToString(ofGetFrameNum()) + ".mov");
			} else {
				movieOutput.finishMovie();
			}
		}
	}
}

const unsigned char testApp::scol[8][3] = {{255, 255, 255},
	{255, 0, 0}, /// red is the warmest
	{255, 255, 0},
	{0, 255, 0},
	{0, 255, 255},
	{0, 0, 255},/// blue is the coolest, but purple comes later
	{255, 0, 255},
	{0, 0, 0}
};

ofColor testApp::makeColor(float f) {
	/// or wrap around?
	if (f > 1.0) f = 1.0;
	if (f < 0.0) f = 0.0;

	int i1 = 0;
	int i2 = 1;
	f = f * (8 - 1);

	while (f > 1.0) {
		f -= 1.0;
		i1++;
		i2++;
	}

	int r = int((1.0 - f) * (float)scol[i1][0] + f * (float)scol[i2][0]);
	int g = int((1.0 - f) * (float)scol[i1][1] + f * (float)scol[i2][1]);
	int b = int((1.0 - f) * (float)scol[i1][2] + f * (float)scol[i2][2]);

	ofColor rv;
	rv.r = r;
	rv.g = g;
	rv.b = b;
	return rv;
}
