#pragma once

#include <ofMain.h>
#include <vector>

class ofAppGlutWindow;

namespace ofxDome {

	class Screen;

	class MultiScreenApp : public ofBaseApp {
	private:
		int activeScreen;
		int wholeWidth, wholeHeight;
		int screenWidth, screenHeight;
		int tileX, tileY;
		int mouseToScreen(int x, int y) const;
		ofVec2f screenOrigin(int id) const;
		
	protected:
		std::vector< ofPtr<Screen> > screens;
		
	public:
		void setScreenDivision(int tileX, int tileY);
		int getTileX() const { return tileX; };
		int getTileY() const { return tileY; };
		int countScreens() const { return screens.size(); };
		
	public:
		MultiScreenApp();
		virtual ~MultiScreenApp();
		
		virtual void setup();
		void setupScreens();
		virtual void update();
		virtual void draw();
		virtual void exit();
		
		virtual void keyPressed( int key );
		virtual void keyReleased( int key );
		
		virtual void mouseMoved( int x, int y );
		virtual void mouseDragged( int x, int y, int button );
		virtual void mousePressed( int x, int y, int button );
		virtual void mouseReleased();
		virtual void mouseReleased(int x, int y, int button );
	};
	
}