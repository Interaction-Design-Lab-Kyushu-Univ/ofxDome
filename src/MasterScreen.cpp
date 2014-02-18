#include "MasterScreen.h"

using namespace ofxDome;

MasterScreen::MasterScreen(ofPtr<MeshControl> meshControl1, ofPtr<MeshControl> meshControl2, ofPtr< std::vector<ofFbo> > fbos)
: fbos(fbos)
{
	meshControls[0] = meshControl1;
	meshControls[1] = meshControl2;
	selectedRect = -1;
}

void MasterScreen::setup() {
	rect[0] = ofRectangle(0.0f, 0.0f, getWidth()*0.5f, getHeight()*0.5f);
	rect[1] = ofRectangle(getWidth()*0.5f, 0.0f, getWidth()*0.5f, getHeight()*0.5f);
	fbosRect = ofRectangle(0.0f, getHeight()*0.5f, getWidth(), getHeight()*0.5f);
}

void MasterScreen::update() {
	
}

ofRectangle MasterScreen::getFillRect(const ofRectangle& area, float width, float height) {
	if (area.width * height > width * area.height) {
		// area is wider than source
		return ofRectangle(area.position, area.height * width / height, area.height);
	} else {
		return ofRectangle(area.position, area.width, area.width * height / width);
	}
}

void MasterScreen::draw() {
	meshControls[0]->draw(rect[0], true);
	meshControls[1]->draw(rect[1], true);
	
	switch (fbos->size()) {
	case 1:
		{
			ofRectangle rect = getFillRect(fbosRect, fbos->at(0).getWidth(), fbos->at(0).getHeight());
			fbos->at(0).draw(rect.x, rect.y, rect.width, rect.height);
			break;
		}
	case 5:
		{
			for (int y = 0; y < 2; y++) {
				for (int x = 0; x < 3; x++) {
					int n = y * 3 + x;
					if (n >= 5) break;
					
					ofRectangle rect(fbosRect.getMin(), fbosRect.getWidth() / 3.0f, fbosRect.getHeight() / 2.0f);
					rect.translate(rect.width * x, rect.height * y);
					ofRectangle fillrect = getFillRect(rect, fbos->at(n).getWidth(), fbos->at(n).getHeight());
					fbos->at(n).draw(fillrect.x, fillrect.y, fillrect.width, fillrect.height);
				}
			}
			break;
		}
	}
}

void MasterScreen::exit() {
	
}

void MasterScreen::keyPressed(int key) {

}

void MasterScreen::keyReleased(int key) {
	
}

void MasterScreen::mouseMoved(int x, int y) {
	if (rect[0].inside(ofPoint(x, y))) {
		meshControls[0]->mouseMoved(rect[0], ofVec2f(x, y));
	} else if (rect[1].inside(ofPoint(x, y))) {
		meshControls[1]->mouseMoved(rect[1], ofVec2f(x, y));
	}
}

void MasterScreen::mousePressed(int x, int y, int button) {
	if (rect[0].inside(ofPoint(x, y))) {
		meshControls[0]->mousePressed(rect[0], ofVec2f(x, y));
		selectedRect = 0;
	} else if (rect[1].inside(ofPoint(x, y))) {
		meshControls[1]->mousePressed(rect[1], ofVec2f(x, y));
		selectedRect = 1;
	}
}

void MasterScreen::mouseReleased(int x, int y, int button) {
	if (selectedRect != -1) {
		meshControls[selectedRect]->mouseReleased(rect[selectedRect], ofVec2f(x, y));
		selectedRect = -1;
	}
}

void MasterScreen::mouseDragged(int x, int y, int button) {
	if (selectedRect != -1) {
		meshControls[selectedRect]->mouseDragged(rect[selectedRect], ofVec2f(x, y));
	}
}