#pragma once

#include <ofBaseApp.h>

namespace ofxDome {
	
	class MultiScreenApp;

	/**
	 * this is a base class for each division of MultiScreenApp
	 */
	class Screen : public ofBaseApp {
		friend class MultiScreenApp;
	private:
		MultiScreenApp* parent;
		
		virtual void windowResized(int w, int h) final {};
		virtual void dragEvent(ofDragInfo dragInfo) final {};
		virtual void gotMessage(ofMessage msg) final {};
		virtual void windowEntry ( int state ) final {};
		
	public:
		Screen();
		int getWidth() const;
		int getHeight() const;
	};

}