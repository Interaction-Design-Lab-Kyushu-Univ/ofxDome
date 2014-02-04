#include "Screen.h"
#include "ofMain.h"
#include "MultiScreenApp.h"

using namespace ofxDome;

Screen::Screen()
: parent(NULL)
{
	
}

int Screen::getWidth() const {
	return ofGetWindowSize().x / parent->getTileX();
}

int Screen::getHeight() const {
	return ofGetWindowSize().y / parent->getTileY();
}