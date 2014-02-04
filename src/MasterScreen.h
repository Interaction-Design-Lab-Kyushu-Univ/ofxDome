#pragma once

#include "ofMain.h"
#include "MeshControl.h"
#include "Screen.h"

namespace ofxDome {
	
	/**
	 * MasterScreen provides a whole view for your dome projection
	 */
	class MasterScreen : public Screen {
		ofRectangle rect[2], fbosRect;
		ofPtr<MeshControl> meshControls[2];
		ofPtr< std::vector<ofFbo> > fbos;
		
		int selectedRect;
		
		static ofRectangle getFillRect(const ofRectangle& area, float width, float height);
		
	public:
		MasterScreen(ofPtr<MeshControl> meshControl1, ofPtr<MeshControl> meshControl2, ofPtr< std::vector<ofFbo> > fbos);
		
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseDragged(int x, int y, int button);
	};
}