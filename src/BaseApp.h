#pragma once

#include "MultiScreenApp.h"
#include "CoordinateSystem.h"
#include "ofMain.h"
#include "MeshControl.h"

namespace ofxDome {
	class ClientScreen;
	class MasterScreen;
	class Mesh;
	
	/**
	 * ofxDome::BaseApp is an implementation of ofBaseApp
	 * Overriding this class instead of  is almost same as holding ofxDome instance
	 */
	class BaseApp : public MultiScreenApp {
	protected:
		ofPtr<CoordinateSystem> coordinateSystem;
		ofPtr<std::vector<ofFbo> > fbos;
		
		ofPtr<ClientScreen> clientScreens[2];
		ofPtr<MasterScreen> masterScreen;
		
		ofPtr<Mesh> meshes[2];
		ofPtr<MeshControl> meshControls[2];
		
		bool isUIEnabled;
		
	public:
		BaseApp();
		
		void setup();
		
		void setCoordinateSystem(ofPtr<CoordinateSystem> cs);
		
		void setMeshVisiblity(bool enabled);
		bool getMeshVisiblity() const;
		
		ofFbo& getFbo(int index);
		
		std::string getMeshCompositionString() const;
		bool loadMeshCompositionString(const std::string& string);
		
		virtual void draw();
	};
}