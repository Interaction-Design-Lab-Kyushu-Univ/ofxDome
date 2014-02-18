#include "ofMain.h"
#include "testApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){
    ofPtr<ofAppGLFWWindow> window = ofPtr<ofAppGLFWWindow>(new ofAppGLFWWindow());
    window->setMultiDisplayFullscreen(true);
    
	// ofSetupOpenGL(window, 1200, 250, OF_WINDOW);
    ofSetupOpenGL(window, 1280, 800, OF_FULLSCREEN);
    ofSetWindowShape(ofGetScreenWidth()*3, ofGetScreenHeight());
    
	ofRunApp(new testApp());
}
