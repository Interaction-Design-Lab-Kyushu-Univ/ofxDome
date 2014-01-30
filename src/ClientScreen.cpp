#include "ClientScreen.h"
#include "MasterScreen.h"

using namespace ofxDome;

ClientScreen::ClientScreen(int id, ofPtr<MeshControl> meshControl)
: id(id), meshControl(meshControl), isInteractionEnabled(true)
{
	
}

void ClientScreen::setup() {
	rect = ofRectangle(0, 0, getWidth(), getHeight());
}

void ClientScreen::draw() {
	meshControl->draw(rect, isInteractionEnabled);
}

void ClientScreen::setInteractionEnabled(bool value) {
	isInteractionEnabled = value;
}

bool ClientScreen::getInteractionEnabled() const {
	return isInteractionEnabled;
}

void ClientScreen::mouseMoved(int x, int y) {
	if (isInteractionEnabled) {
		meshControl->mouseMoved(rect, ofVec2f(x, y));
	}
}

void ClientScreen::mousePressed(int x, int y, int button) {
	if (isInteractionEnabled) {
		meshControl->mousePressed(rect, ofVec2f(x, y));
	}
}

void ClientScreen::mouseReleased(int x, int y, int button) {
	if (isInteractionEnabled) {
		meshControl->mouseReleased(rect, ofVec2f(x, y));
	}
}

void ClientScreen::mouseDragged(int x, int y, int button) {
	if (isInteractionEnabled) {
		meshControl->mouseDragged(rect, ofVec2f(x, y));
	}
}