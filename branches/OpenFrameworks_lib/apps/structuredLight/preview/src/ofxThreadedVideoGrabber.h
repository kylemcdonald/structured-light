#include "ofVideoGrabber.h"
#include "ofxThread.h"
#include "ofEvents.h"

class ofxThreadedVideoGrabber : public ofVideoGrabber, public ofxThread {
protected:
	int pollRate;
	void threadedFunction() {
		while(threadRunning) {
			lock();
			update();
			unlock();
			if(isFrameNew())
				ofNotifyEvent(frameReceived, *this);
			ofSleepMillis(pollRate);
		}
	}
public:
	ofEvent<ofVideoGrabber> frameReceived;
	ofxThreadedVideoGrabber() {
		pollRate = 1;
	}
	template <class L>
	void setup(int width, int height, L* listener, bool useTexture = false, bool verbose = false) {
		setDesiredFrameRate(60);
		initGrabber(width, height, useTexture);
		addListener(listener);
		startThread(true, verbose);
	}
	void setPollRate(int pollRate) {
		this->pollRate = pollRate;
	}
	int getPollRate() {
		return pollRate;
	}
	template <class L>
	void addListener(L* listener) {
		ofAddListener(frameReceived, listener, &(L::frameReceived));
	}
	void getThreadedPixels(unsigned char* pixels) {
		lock();
		memcpy(pixels, this->pixels, width * height * 3);
		unlock();
	}
};
