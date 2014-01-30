#include "ofMain.h"
#include "testApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){
    ofPtr<ofAppGLFWWindow> window = ofPtr<ofAppGLFWWindow>(new ofAppGLFWWindow());
    window->setMultiDisplayFullscreen(true);
    
	ofSetupOpenGL(window, 1280*3, 800, OF_FULLSCREEN);			// <-------- setup the GL context
	// ofSetupOpenGL(window, 1200, 250, OF_WINDOW);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new testApp());

}
