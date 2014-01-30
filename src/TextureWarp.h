#pragma once

#include "CoordinateSystem.h"
#include "Mesh.h"

namespace ofxDome {
	class TextureWarp {
		ofVbo vbo;
		ofPtr<CoordinateSystem> coordinateSystem;
		ofPtr<Mesh> mesh;
		/*std::vector<ofVec2f> verts, texCoords;
		std::vector<ofIndexType> indices;
		std::vector<bool> errors;*/
		int textureId;
		
	public:
		TextureWarp(ofPtr<CoordinateSystem> cs, ofPtr<Mesh> mesh, int textureId);
		void setMesh(ofPtr<Mesh> mesh);
		void generate();
		void draw();
	};
}