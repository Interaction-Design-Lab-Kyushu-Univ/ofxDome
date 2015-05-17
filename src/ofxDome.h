#pragma once

#include "Types.h"
#include "BaseApp.h"

namespace ofxDome {
	/**
	 * @class ofxDome
	 * @brief this class provides minimal API for dome projection
	 */
	class ofxDome {
		BaseApp* baseApp;
		
	public:
		ofxDome();
		~ofxDome();
		
		/**
		 * Set number of division of the window
		 * This method does nothing after setup
		 * @param x number of rows
		 * @param y number of cols
		 */
		void setScreenDivision(int x, int y);
		
		/**
		 * set coordinate system and its parameters
		 * This method does nothing after setup
		 * @param cs CoordinateSystem
		 */
		
		void setCoordinateSystemPanorama(int fboWidth, int fboHeight, float maxPhi = MATH_PI() * 0.5f);
		
		void setCoordinateSystemPolar(int fboSize, float minPhi = 0.0f);
		
		void setCoordinateSystemCube(int fboSize);
		
		/**
		 * Setup all components
		 */
		void setup();
		
		/**
		 * Enable or disable mesh control on sub screens
		 * This method does nothing before setup
		 * @param visible
		 */
		void setMeshVisiblity(bool visible);
		
		/**
		 * get mesh control
		 * this method does nothing before setup
		 */
		bool getMeshVisiblity() const;
		
		/**
		 * Get mesh composition as string
		 * This method does nothing before setup
		 * @return string representation of vertices' positions
		 */
		std::string getMeshCompositionString() const;
		
		/**
		 * Load meshe's composition from string
		 * This method does nothing before setup
		 * @return whether any error occured
		 */
		bool loadMeshCompositionString(const std::string& string);
		
		/**
		 * Get nth FBO bound for warping
		 * Available index size is defined by coordinate system
		 * This method does nothing before setup
		 * @return instance of ofFbo
		 */
		ofFbo& getFbo(int index);
		
		/**
		 * return number of available FBOs
		 */
		int countFbos() const;
		
		/**
		 * shorthand method
		 */
		ofFbo& beginFbo(int index = 0, bool setupScreen = true) { getFbo(index).begin(setupScreen); return getFbo(index); };
		void endFbo();
		void endFbo(int index) { getFbo(index).end(); };
	};
}