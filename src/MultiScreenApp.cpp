#include "MultiScreenApp.h"
#include "Screen.h"
#include <ofAppGlutWindow.h>

using namespace ofxDome;

MultiScreenApp::MultiScreenApp()
: tileX(1), tileY(1)
{
	ofPoint size = ofGetWindowSize();
	wholeWidth = size.x;
	wholeHeight = size.y;
}

MultiScreenApp::~MultiScreenApp(){
}

void MultiScreenApp::setScreenDivision(int tileX, int tileY) {
	assert(tileX >= 1 && tileY >= 1);
	assert(wholeWidth % tileX == 0);
	assert(wholeHeight % tileY == 0);
	
	this->tileX = tileX;
	this->tileY = tileY;
	
	screens.reserve(tileX * tileY);
}


void MultiScreenApp::setup() {
	ofSetWindowPosition(0, 0);
	
	activeScreen = 0;
	screenWidth = wholeWidth / tileX;
	screenHeight = wholeHeight / tileY;
}

void MultiScreenApp::setupScreens() {
	for (auto screen : screens) {
		if (screen) {
			screen->parent = this;
			screen->setup();
		}
	}
}

void MultiScreenApp::update() {
	for (auto screen : screens) {
		screen->mouseX = mouseX;
		screen->mouseY = mouseY;
		screen->update();
	}
	
}
void MultiScreenApp::draw() {
	ofBackground(0, 0, 0);
	
	int i = 0;
	for (auto screen : screens) {
		ofPushMatrix();
		ofVec2f origin = screenOrigin(i);
		ofViewport(origin.x, origin.y, screenWidth, screenHeight);
		ofScale(ofGetWidth() / screenWidth, ofGetHeight() / screenHeight);
		screen->draw();
		i++;
		ofPopMatrix();
	}
}

void MultiScreenApp::exit() {
	for (auto screen : screens) {
		screen->exit();
	}
}

void MultiScreenApp::keyPressed( int key ) {
	screens[activeScreen]->keyPressed(key);
}

void MultiScreenApp::keyReleased( int key ) {
	screens[activeScreen]->keyReleased(key);
}

int MultiScreenApp::mouseToScreen(int x, int y) const {
	return CLAMP(x / screenWidth, 0, screens.size() - 1);
}

ofVec2f MultiScreenApp::screenOrigin(int id) const {
	int y = id / tileX;
	int x = id - y * tileX;
	return ofVec2f(x * screenWidth, y * screenHeight);
}

void MultiScreenApp::mouseMoved( int x, int y ) {
	activeScreen = mouseToScreen(x, y);
	auto origin = screenOrigin(activeScreen);
	screens[activeScreen]->mouseMoved(x - origin.x, y - origin.y);
}

void MultiScreenApp::mouseDragged( int x, int y, int button ) {
	auto origin = screenOrigin(activeScreen);
	screens[activeScreen]->mouseDragged(x - origin.x, y - origin.y, button);
}

void MultiScreenApp::mousePressed( int x, int y, int button ) {
	activeScreen = mouseToScreen(x, y);
	auto origin = screenOrigin(activeScreen);
	screens[activeScreen]->mousePressed(x - origin.x, y - origin.y, button);
}

void MultiScreenApp::mouseReleased() {
	screens[activeScreen]->mouseReleased();
}

void MultiScreenApp::mouseReleased(int x, int y, int button ) {
	auto origin = screenOrigin(activeScreen);
	screens[activeScreen]->mouseReleased(x - origin.x, y - origin.y, button);
}
