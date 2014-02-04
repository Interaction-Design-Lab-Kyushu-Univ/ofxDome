#pragma once

#include "Screen.h"
#include "ofMain.h"
#include "MeshControl.h"

namespace ofxDome {

	class MasterScreen;

	class ClientScreen : public Screen {
		int id;
		ofRectangle rect;
		ofPtr<MeshControl> meshControl;
		bool isInteractionEnabled;
		
	public:
		ClientScreen(int id, ofPtr<MeshControl> meshControl);
		
		void setInteractionEnabled(bool value);
		bool getInteractionEnabled() const;
		
		void setup();
		void draw();
		
		void mouseMoved(int x, int y);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseDragged(int x, int y, int button);
	};
}