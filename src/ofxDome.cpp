#include "ofxDome.h"
#include "BaseApp.h"
#include "ofMain.h"

#include "ofEventUtils.h"

namespace ofxDome {

	ofxDome::ofxDome()
	: baseApp(NULL)
	{
		baseApp = new BaseApp();
	}
	
	ofxDome::~ofxDome() {
		delete baseApp;
	}

	void ofxDome::setScreenDivision(int x, int y) {
		baseApp->setScreenDivision(x, y);
	}

	void ofxDome::setCoordinateSystemPanorama(int fboWidth, int fboHeight, float maxPhi) {
		baseApp->setCoordinateSystem(ofPtr<CoordinateSystem>(new CoordinateSystemPanorama(ofVec2f(fboWidth, fboHeight), maxPhi)));
	}
	
	void ofxDome::setCoordinateSystemPolar(int fboSize, float minPhi) {
		baseApp->setCoordinateSystem(ofPtr<CoordinateSystem>(new CoordinateSystemPolar(minPhi, fboSize)));
	}
	
	void ofxDome::setCoordinateSystemCube(int fboSize) {
		baseApp->setCoordinateSystem(ofPtr<CoordinateSystem>(new CoordinateSystemCube(fboSize)));
	}

	
	void ofxDome::setup() {
		ofAddListener(ofEvents().update, (ofBaseApp*)baseApp, &ofBaseApp::update);
		ofAddListener(ofEvents().draw, (ofBaseApp*)baseApp, &ofBaseApp::draw);
		ofAddListener(ofEvents().exit, (ofBaseApp*)baseApp, &ofBaseApp::exit);
		ofAddListener(ofEvents().keyPressed, (ofBaseApp*)baseApp, &ofBaseApp::keyPressed);
		ofAddListener(ofEvents().keyReleased, (ofBaseApp*)baseApp, &ofBaseApp::keyReleased);
		ofAddListener(ofEvents().mouseMoved, (ofBaseApp*)baseApp, &ofBaseApp::mouseMoved);
		ofAddListener(ofEvents().mouseDragged, (ofBaseApp*)baseApp, &ofBaseApp::mouseDragged);
		ofAddListener(ofEvents().mousePressed, (ofBaseApp*)baseApp, &ofBaseApp::mousePressed);
		ofAddListener(ofEvents().mouseReleased, (ofBaseApp*)baseApp, &ofBaseApp::mouseReleased);
		
		baseApp->setup();
	}
	
	void ofxDome::setMeshVisiblity(bool enabled) {
		baseApp->setMeshVisiblity(enabled);
	}
	
	bool ofxDome::getMeshVisiblity() const {
		return baseApp->getMeshVisiblity();
	}

	ofFbo& ofxDome::getFbo(int index) {
		return baseApp->getFbo(index);
	}
	
	int ofxDome::countFbos() const {
		return baseApp->countFbos();
	}
	
	std::string ofxDome::getMeshCompositionString() const {
		return baseApp->getMeshCompositionString();
	}
	
	bool ofxDome::loadMeshCompositionString(const std::string& string) {
		return baseApp->loadMeshCompositionString(string);
	}

	void ofxDome::endFbo() {
		for (int i = 0; i < countFbos(); i++) {
			getFbo(i).end();
		}
	}
}