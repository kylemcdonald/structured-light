#include "testApp.h"

/*
 abstract into a class:
 pass a vector of images + vector of exposure times
 run separately for each channel
 return curves in 0-255 range

 export curves files to .txt
 add class that does fast lut remap of incoming images
 and can load/save curves files

 save out all the images
 check in processing that things are linear

 add code for averaging multiple passes
*/

void testApp::setup() {
	ofMousePressed = false;

	levels = 256;
	sampledLocations = 128;
	sampledExposures = 32;
	float smoothness = 4.0f;

	inputCurves = cvCreateMat(sampledLocations, sampledExposures, CV_8UC1);
	inputExposures = cvCreateMat(sampledExposures, 1, CV_32FC1);
	weighting = cvCreateMat(levels, 1, CV_8UC1);

	unsigned char* inputCurvesPtr = (unsigned char*) cvPtr2D(inputCurves, 0, 0);
	float* inputExposuresPtr = (float*) cvPtr1D(inputExposures, 0);
	unsigned char* weightingPtr = (unsigned char*) cvPtr1D(weighting, 0);

	vector<float> exposureTimes;

	ofImage cur;
	for(int i = 0; i < sampledExposures; i++) {
		int j = (i * 512) / sampledExposures;
		cout << "Loading " << j << endl;
		cur.loadImage("rpi/" + ofToString(j) + ".png");
		images.push_back(cur);
		float exposureTime = ((j * 270 / 511)) + 1; // in terms of rows (PS3Eye)
		exposureTimes.push_back(exposureTime);
		*(inputExposuresPtr++) = logf(exposureTime) / logf(M_E); // log base e
	}
	cout << "Loaded " << images.size() << " images." << endl;

	gammaCalibration.calibrate(images, exposureTimes);

	int* locations = new int[sampledLocations];
	int totalPixels = (int) cur.getWidth() * (int) cur.getHeight();

	// for the locations, we should find some evenly spaced samples
	// first, do a histogram sort of the pixels in the middle image
	// randomly pick n values, evenly spaced from the histogram
	cout << "Sorting the middle image." << endl;
	vector< vector<int> > sorted;
	sorted.resize(levels);
	unsigned char* middle = images[images.size() / 2].getPixels();
	for(int i = 0; i < totalPixels; i++) {
		unsigned char curColor = middle[i * 3];
		sorted[curColor].push_back(i);
	}
	cout << "Done sorting." << endl;

	cout << "Checking valid intensities." << endl;
	vector<unsigned char> valid;
	for(int i = 0; i < levels; i++)
		if(sorted[i].size() > 0)
			valid.push_back(i);
	cout << "Found " << valid.size() << " valid intensities." << endl;

	cout << "Grabbing " << sampledLocations << " random equidistant locations." << endl;
	for(int i = 0; i < sampledLocations; i++) {
		float j = (float) i / (float) sampledLocations;
		int curIntensity = valid[(int) (j * valid.size())];
		vector<int>& curLocations = sorted[curIntensity];
		locations[i] = curLocations[(int) ofRandom(0, curLocations.size())];
	}

	// this part is fairly inefficient, filling up Z requires jumping between images
	for(int i = 0; i < sampledLocations; i++) {
		int curLocation = locations[i] * 3;
		for(int j = 0; j < sampledExposures; j++) {
			unsigned char* curPixels = images[j].getPixels();
			*(inputCurvesPtr++) = curPixels[curLocation];
		}
	}
	delete [] locations;

	// generate the weighting (hat) function
	for(int z = 0; z < levels / 2; z++)
		*(weightingPtr++) = z;
	for(int z = levels / 2; z < levels; z++)
		*(weightingPtr++) = levels - z;

	long sizeA1 = sampledLocations * sampledExposures + levels + 1;
	long sizeA2 = levels + sampledLocations;
	A = cvCreateMat(sizeA1, sizeA2, CV_32FC1);
	b = cvCreateMat(sizeA1, 1, CV_32FC1);
	x = cvCreateMat(sizeA2, 1, CV_32FC1);

	cout << "A(" << sizeA1 << ", " << sizeA2 << ")" << endl;
	cout << "b(" << sizeA1 << ", 1)" << endl;
	cout << "x(" << sizeA2 << ", 1)" << endl;

	weightingPtr = (unsigned char*) cvPtr1D(weighting, 0); // reset ptr
	inputExposuresPtr = (float*) cvPtr1D(inputExposures, 0); // reset ptr
	int k = 0;
	for(int i = 0; i < sampledLocations; i++) {
		for(int j = 0; j < sampledExposures; j++) {
			unsigned char curZ = *cvPtr2D(inputCurves, i, j);
			unsigned char wij = weightingPtr[curZ];
			*((float*) cvPtr2D(A, k, curZ)) = wij;
			*((float*) cvPtr2D(A, k, levels + i)) = -wij;
			*((float*) cvPtr1D(b, k)) = wij * inputExposuresPtr[j];
			k++;
		}
	}

	*((float*) cvPtr2D(A, k, 129)) = 1;
	k++;

	for(int i = 0; i < levels - 2; i++) {
		*((float*) cvPtr2D(A, k, i)) = smoothness * weightingPtr[i];
		*((float*) cvPtr2D(A, k, i + 1)) = -2 * smoothness * weightingPtr[i];
		*((float*) cvPtr2D(A, k, i + 2)) = smoothness * weightingPtr[i];
		k++;
	}

	cout << "Solving SVD..." << endl;
	cvSolve(A, b, x, CV_SVD);

	ofstream file;
	file.open("curve.csv");
	float* xptr = (float*) cvPtr1D(x, 0);
	for(int i = 0; i < levels; i++)
		file << i << "," << *(xptr++) << endl;
	file.close();

	cout << "Done." << endl;
}

testApp::~testApp() {
	cvReleaseMat(&A);
	cvReleaseMat(&b);
	cvReleaseMat(&x);

	cvReleaseMat(&inputCurves);
	cvReleaseMat(&inputExposures);
	cvReleaseMat(&weighting);
}

void testApp::update() {
}

void testApp::draw() {
	ofBackground(255, 255, 255);
	ofSeedRandom(0);

	unsigned char* inputCurvesPtr = (unsigned char*) cvPtr2D(inputCurves, 0, 0);
	float* inputExposuresPtr = (float*) cvPtr1D(inputExposures, 0);
	float* xPtr = (float*) cvPtr1D(x, 0);

	float maxExposure = inputExposuresPtr[sampledExposures - 1];
	int width = (int) ofGetWidth();
	int height = (int) ofGetHeight();
	for(int i = 0; i < sampledLocations; i++) {
		ofSetColor(
			(int) ofRandom(0, 255),
			(int) ofRandom(0, 255),
			(int) ofRandom(0, 255));
		if(ofMousePressed) {
			glBegin(GL_LINE_STRIP);
			for(int j = 0; j < sampledExposures; j++) {
				glVertex2f(
					inputCurvesPtr[i * sampledExposures + j] * width / levels,
					height - inputExposuresPtr[j] * height / maxExposure);
			}
			glEnd();
		} else {
			glBegin(GL_POINTS);
			float curExposure = xPtr[levels + i];
			for(int j = 0; j < sampledExposures; j++) {
				glVertex2f(
					inputCurvesPtr[i * sampledExposures + j] * width / levels,
					(height / 2) - (inputExposuresPtr[j] + curExposure) * mouseY);
			}
			glEnd();
		}
	}

	if(!ofMousePressed) {
		ofSetColor(255, 0, 0, 128);
		glBegin(GL_LINE_STRIP);
		for(int i = 0; i < levels; i++) {
			glVertex2f(
				i * width / levels,
				(height / 2) - xPtr[i] * mouseY);
		}
		glEnd();
	}
}

void testApp::mousePressed(int x, int y, int button) {
	ofMousePressed = true;
}

void testApp::mouseReleased(int x, int y, int button) {
	ofMousePressed = false;
}
