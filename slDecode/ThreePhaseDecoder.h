#pragma once

#include "ofImage.h"
#include "FastQueue.h"

struct intPoint {
	int x;
	int y;
};

class ThreePhaseDecoder {
public:
	void setup(int width, int height, float zskew, float zscale, float noiseTolerance = .15);
	void loadImages(
		string phase1Filename,
		string phase2Filename,
		string phase3Filename);
	void loadImage(int phase, string filename);
	void setImage(int phase, ofImage& img);
	void updateColor();
	int getWidth();
	int getHeight();
	bool** getMask();
	float** getDepth();
	unsigned char** getColor();
	void decode();
	bool ready();
	~ThreePhaseDecoder();

	ofImage img[3];
	ofImage imgColor[3];

	bool wide;
	float noiseTolerance, zskew, zscale;
private:
	void phaseWrap();
	inline float phaseWrap(
		const unsigned char& phase1,
		const unsigned char& phase2,
		const unsigned char& phase3);
	void phaseUnwrap();
	inline void phaseUnwrap(const float& r, int x, int y);
	void makeDepth();

	int width, height;

	FastQueue<intPoint> toProcess;

	float** wrapphase;
	bool** mask;
	bool** process;
	float** depth;
	unsigned char** color;
};
