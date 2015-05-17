#include "TextureWarp.h"

using namespace ofxDome;

TextureWarp::TextureWarp(ofPtr<CoordinateSystem> cs, ofPtr<Mesh> mesh, int textureId)
: coordinateSystem(cs), mesh(mesh), textureId(textureId)
{
	
}

void TextureWarp::setMesh(ofPtr<Mesh> mesh) {
	this->mesh = mesh;
}


void TextureWarp::generate() {
	vbo.clear();
	
	std::vector<ofVec2f> verts;
	std::vector<bool> errors;
	std::vector<ofIndexType> indices;
	std::vector<ofVec2f> texCoords;
	verts.reserve(UV_DIVISION * UV_DIVISION);
	errors.reserve(UV_DIVISION * UV_DIVISION);
	indices.reserve(UV_DIVISION * UV_DIVISION * 2 * 3);
	texCoords.reserve(UV_DIVISION * UV_DIVISION);
	
	ofVec2f maxTexCoords = coordinateSystem->getMaxTexCoords(textureId);
	
	for (int j = 0; j < UV_DIVISION; j++) {
		float v = (float)j / (UV_DIVISION - 1);
		for (int i = 0; i < UV_DIVISION; i++) {
			float u = (float)i / (UV_DIVISION - 1);
			
			PolarCoords pc = coordinateSystem->convertUVFast(ofVec2f(u, v), textureId);
			ofVec2f sp;
			bool success = mesh->convertPolarCoordsToScreenPosition(pc, &sp);
			if (success) {
				verts.push_back(sp);
				texCoords.push_back(ofVec2f(u * maxTexCoords.x, v * maxTexCoords.y));
				errors.push_back(false);
			} else {
				verts.push_back(ofVec2f());
				texCoords.push_back(ofVec2f());
				errors.push_back(true);
			}
		}
	}
	
	for (int j = 0; j < UV_DIVISION - 1; j++) {
		for (int i = 0; i < UV_DIVISION - 1; i++) {
			if (!errors[i * UV_DIVISION + j] && !errors[i * UV_DIVISION + j+1] && !errors[(i+1) * UV_DIVISION + j+1]) {
				indices.push_back(i * UV_DIVISION + j);
				indices.push_back(i * UV_DIVISION + j+1);
				indices.push_back((i+1) * UV_DIVISION + j+1);
			}
			if (!errors[i * UV_DIVISION + j] && !errors[(i+1) * UV_DIVISION + j] && !errors[(i+1) * UV_DIVISION + j+1]) {
				indices.push_back(i * UV_DIVISION + j);
				indices.push_back((i+1) * UV_DIVISION + j);
				indices.push_back((i+1) * UV_DIVISION + j+1);
			}
		}
	}
	
	vbo.setVertexData(verts.data(), verts.size(), GL_DYNAMIC_DRAW);
	vbo.setIndexData(indices.data(), indices.size(), GL_DYNAMIC_DRAW);
	vbo.setTexCoordData(texCoords.data(), texCoords.size(), GL_DYNAMIC_DRAW);
}

void TextureWarp::draw() {
	vbo.drawElements(GL_TRIANGLES, vbo.getNumIndices());
}