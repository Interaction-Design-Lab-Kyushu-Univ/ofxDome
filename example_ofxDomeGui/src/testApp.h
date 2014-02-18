#pragma once

#include "ofMain.h"
#include "ofxDome.h"
#include "ofxGui.h"

class ImageOrVideo {
    bool isImage;
    ofImage image;
    ofVideoPlayer video;
    
public:
    bool load(const std::string& path);
    void update();
    void draw(float width, float height);
    int getWidth() const;
    int getHeight() const;
    void play();
};

class testApp : public ofBaseApp{
    ofxDome::ofxDome dome;
    ofxPanel gui;
    ofxButton panoramaButton, polarButton, cubeButton;
    
    ImageOrVideo images[5];

	public:
		void setup();
		void update();
		void draw();
        void drawGui(ofEventArgs& e);
    
        void onPanoramaButtonPressed();
        void onPolarButtonPressed();
        void onCubeButtonPressed();

		void keyPressed(int key);
		void mouseReleased(int x, int y, int button);
};
