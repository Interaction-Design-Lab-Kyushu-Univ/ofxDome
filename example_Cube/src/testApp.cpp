#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	dome.setScreenDivision(3, 1);
	dome.setCoordinateSystemCube(1024);
	dome.setup();
	
	dome.loadMeshCompositionString(ofBufferFromFile("mesh.txt").getText());
	
	images[ofxDome::CUBE_TEXTURE_TOP] = ofImage("cube_top.jpg");
	images[ofxDome::CUBE_TEXTURE_FRONT] = ofImage("cube_front.jpg");
	images[ofxDome::CUBE_TEXTURE_LEFT] = ofImage("cube_left.jpg");
	images[ofxDome::CUBE_TEXTURE_RIGHT] = ofImage("cube_right.jpg");
	images[ofxDome::CUBE_TEXTURE_BACK] = ofImage("cube_back.jpg");
}

//--------------------------------------------------------------
void testApp::update() {

}

//--------------------------------------------------------------
void testApp::draw() {
	for (int i = 0; i < ofxDome::CUBE_TEXTURES_NUM; i++) {
		dome.beginFbo(i);
		images[i].draw(0, 0, 1024, 1024);
		dome.endFbo(i);
	}
}

//--------------------------------------------------------------
void testApp::keyPressed( int key ) {
	switch (key) {
	case 'l':
		dome.setMeshVisiblity(!dome.getMeshVisiblity());
		break;
	case 'f':
		ofToggleFullscreen();
		break;
	}
}

void testApp::mouseReleased(int x, int y, int button) {
	ofBuffer buffer(dome.getMeshCompositionString());
	ofBufferToFile("mesh.txt", buffer);
}