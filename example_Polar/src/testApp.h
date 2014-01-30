#pragma once

#include "ofMain.h"
#include "ofxDome.h"

class testApp : public ofBaseApp {
	
	ofxDome::ofxDome dome;
	ofImage image;
	
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	
	void mouseReleased(int x, int y, int button);
};
