#pragma once

#include <vector>
#include "ofMain.h"
#include "Mesh.h"

namespace ofxDome {
	class Gradation {
	public:
		virtual ~Gradation() {};
		virtual void generate() = 0;
		virtual void draw() = 0;
	};
	
	class QuarterSphereGradation : public Gradation {
		ofVbo vbo;
		ofPtr<QuarterSphereMesh> mesh;
		
	public:
		QuarterSphereGradation();
		QuarterSphereGradation(ofPtr<QuarterSphereMesh> mesh);
		
		void setMesh(ofPtr<QuarterSphereMesh> mesh);
		void generate();
		void draw();
	};
}