#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	dome.setScreenDivision(3, 1);
	// dome.setCoordinateSystemPanorama(2048, 512);
	dome.setCoordinateSystemPolar(2048);
	// dome.setCoordinateSystemCube(1024);
	dome.setup();
	
	dome.loadMeshCompositionString(ofBufferFromFile("mesh.txt").getText());
	
	// image = ofImage("skydome2048.jpg");
	image = ofImage("polar.jpg");
	
	/*images[ofxDome::CUBE_TEXTURE_TOP] = ofImage("cube_top.jpg");
	images[ofxDome::CUBE_TEXTURE_FRONT] = ofImage("cube_front.jpg");
	images[ofxDome::CUBE_TEXTURE_LEFT] = ofImage("cube_left.jpg");
	images[ofxDome::CUBE_TEXTURE_RIGHT] = ofImage("cube_right.jpg");
	images[ofxDome::CUBE_TEXTURE_BACK] = ofImage("cube_back.jpg");*/
}

//--------------------------------------------------------------
void testApp::update() {

}

//--------------------------------------------------------------
void testApp::draw() {
	ofFbo& fbo = dome.getFbo(0);
	fbo.begin();
	// image.draw(0.0f, 0.0f, fbo.getWidth(), fbo.getHeight() * 2.0f);
	image.draw(0.0f, 0.0f, fbo.getWidth(), fbo.getHeight());
	fbo.end();
	
	
	/*for (int i = 0; i < ofxDome::CUBE_TEXTURES_NUM; i++) {
		dome.getFbo(i).begin();
		images[i].draw(0, 0, 1024, 1024);
		dome.getFbo(i).end();
	}*/
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