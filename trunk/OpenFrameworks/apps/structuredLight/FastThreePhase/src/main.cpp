#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, inputWidth, inputHeight * 2, OF_WINDOW);
	ofRunApp(new testApp());
}
