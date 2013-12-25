#pragma once

#include <ofMain.h>

#include "interpolation.h"
#include "Types.h"
#include "CoordinateSystem.h"

namespace ofxDome {

	class ClientScreen;
	class Mesh;
	
	class MeshVert {
	public:
		MeshVert() {};
		MeshVert(const ofVec2f& screenPosition, const PolarCoords& polarCoords);
		MeshVert(const ofVec2f& screenPosition, const ofVec2f& xy);
		ofVec2f screenPosition;
		ofVec2f xy;
	};
	
	class MeshLine {
		friend class Mesh;
		
		Mesh* mesh;
		mutable bool shouldRegenerateScreenPositionInterpolation;
		mutable bool shouldRegenerateXYInterpolation;
		mutable interpolation_func_t interpolationScreenX, interpolationScreenY;
		mutable interpolation_func_t interpolationX, interpolationY;
		
		void generateScreenPositionInterpolation() const;
		void generateXYInterpolation() const;
	public:
		MeshLine(Mesh* mesh = NULL);
		std::vector<int> vertIndices;
		ofVec2f getInterpolatedScreenPosition(float t) const;
		MeshVert getInterpolatedMeshVert(float t) const;
		bool setAsDirty();
		void draw(int smooth);
	};

	class Mesh {
	protected:
		std::vector<MeshVert> verts;
		std::vector<MeshLine> lines;
		std::vector<int> triIndices;
		std::vector<int> quadIndices;
		
		Mesh();
		
		bool parseCompositionString(const std::string& str);
		
	public:
		virtual ~Mesh();
		
		MeshVert getVert(int index) const;
		int getVertsNum() const;
		ofVec2f moveVert(int index, ofVec2f screenPosition);

		const MeshLine& getLine(int index) const;
		int getLinesNum() const;
		
		void drawLines(int smooth);
		
		ofVec2f convertPolarCoordsToScreenPosition(const PolarCoords& pc, bool* error) const;
		
		// return true if error occurs
		virtual bool loadCompositionString(const std::string& str) = 0;
		virtual std::string getCompositionString() const;
	};
	
	class QuarterSphereMesh : public Mesh {
		int horizontalDivision, verticalDivision;
		void generateLines();
		void generateFaces();
		
		QuarterSphereMesh();
		QuarterSphereMesh(float centerTheta, int horizontalDivsion, int verticalDivision);
	public:
		int getHorizontalDivision() const { return horizontalDivision; };
		int getVerticalDivision() const { return verticalDivision; };
		
		const MeshLine& getTopLine() const;
		const MeshLine& getNthHorizontalLine(int) const;
		const MeshLine& getNthVerticalLine(int) const;
		
		ofPtr<QuarterSphereMesh> createDivision();
		static ofPtr<QuarterSphereMesh> create(float centerTheta, int horizontalDivsion = 4, int verticalDivision = 3);
		
		virtual bool loadCompositionString(const std::string& str);
	};
}