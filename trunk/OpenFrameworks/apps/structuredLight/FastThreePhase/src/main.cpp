#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, inputWidth, inputHeight, OF_WINDOW);
	ofRunApp(new testApp());
}
