/*
	model (ofxStructuredLight)
		good
	view (vbo based rendering)
		only call decode() once per 3d frame unless told to update()
	controller (ofxControlPanel)
		implement connections to m+v -- maybe import/export just in xml now, decode in control panel?
		implement importer + exporter wrappers
		eventually add file dialog... for now specify via xml
*/

#pragma once

#include "ofMain.h"
#include "ofxEasyCam.h"
#include "ofxStructuredLight.h"
#include "ofxAutoControlPanel.h"
#include "ofxQtVideoSaver.h"
#include "ofxNumericalDirList.h"
#include "ofxVectorMath.h"

#include <fstream>

class testApp : public ofBaseApp {

public:
	~testApp();
	void setup();
	void update();
	void draw();
	void keyPressed(int key);

	void drawCloud();
	void drawMesh();

	void getBounds(ofxPoint3f& min, ofxPoint3f& max);
	void boxOutline(ofxPoint3f min, ofxPoint3f max);
	void drawAxes(float size);

	void nextFrame();
	void jumpTo(unsigned frame);
	void setupInput();

	ofxEasyCam camera;

	ThreePhaseDecoder* threePhase;

	string inputDir;

	simpleFileLister inputList;
	ofxNumericalDirList imageList;
	ofVideoPlayer movieInput;
	bool usingDirectory;
	unsigned totalImages;

	ofxAutoControlPanel panel;
	vector<string> styles, exportFormats;
	bool hidden;

	float lastDepthScale, lastDepthSkew;
	float lastFilterMin, lastFilterMax;
	int lastRangeThreshold, lastOrientation;
	float lastGamma;
	float lastJumpTo;
	int lastCameraRate, lastCameraOffset;
	bool lastPhasePersistence;

	/// visualizations
	bool redraw;
	ofImage rangeIm;
	ofImage phaseWrapped;
	ofImage phaseUnwrapped;
	ofImage depthIm;
	ofImage unwrapOrderIm;
	ofImage phaseWrappedPlot;
	ofImage sourceImagesPlot;

	int sequenceFrame;

	ofxQtVideoSaver movieOutput;
	ofImage screenCapture;

	static const unsigned char scol[8][3];

	/// expects a 0-1.0 float
	ofColor makeColor(float f);
};
