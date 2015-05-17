#pragma once

#include "Mesh.h"
#include "Types.h"
#include "TextureWarp.h"
#include "Gradation.h"

namespace ofxDome {

	class MeshControl {
	public:
		static const int NOT_SELECTED = -1;
		
	private:
		int selectedPoint;
		bool isDragging;
		
		ofPtr<QuarterSphereMesh> mesh;
		ofPtr<QuarterSphereMesh> dividedMesh;
		
		std::vector<ofPtr<TextureWarp> > textureWarps;
		ofPtr<std::vector<ofFbo> > fbos;
		ofPtr<CoordinateSystem> coordinateSystem;
		ofPtr<QuarterSphereGradation> gradation;
		
		ofVec2f normalizePosition(const ofRectangle& rect, const ofVec2f& pos) const;
		ofVec2f inverseNormalizedPosition(const ofRectangle& rect, const ofVec2f& pos) const;
		
	public:
		MeshControl(ofPtr<QuarterSphereMesh> mesh, ofPtr<CoordinateSystem> cs, ofPtr<std::vector<ofFbo> > fbos);
		
		/* return true if a control point is clicked */
		bool mousePressed(const ofRectangle& rect, const ofVec2f& pos);
		void mouseMoved(const ofRectangle& rect, const ofVec2f& pos);
		void mouseReleased(const ofRectangle& rect, const ofVec2f& pos);
		void mouseDragged(const ofRectangle& rect, const ofVec2f& pos);
		
		void notifyMeshUpdated();
		
		void draw(const ofRectangle& rect, bool drawLines);
	};
	
}
