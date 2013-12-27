#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	dome.setScreenDivision(3, 1);
	dome.setCoordinateSystemPolar(2048);
	dome.setup();
	
	dome.loadMeshCompositionString(ofBufferFromFile("mesh.txt").getText());
	
	image = ofImage("polar.jpg");
}

//--------------------------------------------------------------
void testApp::update() {

}

//--------------------------------------------------------------
void testApp::draw() {
	ofFbo& fbo = dome.beginFbo(0);
	image.draw(0.0f, 0.0f, fbo.getWidth(), fbo.getHeight());
	dome.endFbo(0);
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