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
		float halfWidth;
		
	public:
		QuarterSphereGradation();
		QuarterSphereGradation(ofPtr<QuarterSphereMesh> mesh, float width);
		
		void setMesh(ofPtr<QuarterSphereMesh> mesh);
		void setWidth(float width);
		void generate();
		void draw();
	};
}