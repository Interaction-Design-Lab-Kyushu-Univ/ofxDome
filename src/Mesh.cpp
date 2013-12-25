#include "Mesh.h"
#include "interpolation.h"
#include "ClientScreen.h"

using namespace ofxDome;

namespace {
	template <typename T> std::vector<T>& operator <<(std::vector<T>& vec, T val) {
		vec.push_back(val);
		return vec;
	}
	
	float cross(const ofVec2f& a, const ofVec2f &b) {
		return a.x * b.y - b.x * a.y;
	}
	
	bool isInsideTriangle(const ofVec2f& p, const ofVec2f& a, const ofVec2f& b, const ofVec2f& c) {
		float c1 = cross(p - a, b - a);
		float c2 = cross(p - b, c - b);
		float c3 = cross(p - c, a - c);
		
		if (c1 <= 0.01f && c2 <= 0.01f && c3 <= 0.01f) return true;
		if (c1 >= -0.01f && c2 >= -0.01f && c3 >= -0.01f) return true;
		return false;
	}
	
	ofVec2f triangleToScreenPosition(const ofVec2f& p, const MeshVert& a, const MeshVert& b, const MeshVert& c, bool* error) {
		ofVec2f _p = p - a.xy;
		ofVec2f _b = b.xy - a.xy;
		ofVec2f _c = c.xy - a.xy;
		
		float det = (_b.x * _c.y - _b.y * _c.x);
		
		if (abs(det) < 0.01f) {
			*error = true;
			return ofVec2f();
		}
		
		float s = (_p.x * _c.y - _p.y * _c.x) / det;
		float t = - (_p.x * _b.y - _p.y * _b.x) / det;
		
		*error = false;
		return a.screenPosition + s * (b.screenPosition - a.screenPosition) + t * (c.screenPosition - a.screenPosition);
	}
	
	std::ostream& operator<< (std::ostream& stream, const MeshLine& line) {
		for (int i = 0; i < line.vertIndices.size(); i++) {
			stream << line.vertIndices[i] << ",";
		}
		return stream;
	}
}

MeshLine::MeshLine(Mesh* mesh)
: shouldRegenerateScreenPositionInterpolation(true), shouldRegenerateXYInterpolation(true), mesh(mesh)
{
}

void MeshLine::generateScreenPositionInterpolation() const {
	std::vector<ofVec2f> xPoints, yPoints;
	xPoints.reserve(vertIndices.size() + 2);
	yPoints.reserve(vertIndices.size() + 2);
	
	float dt = 1.0f / (vertIndices.size() - 1);
	
	for (int i = 0; i < vertIndices.size(); i++) {
		float t = (float)i / (vertIndices.size() - 1);
		ofVec2f screenPos = mesh->getVert(vertIndices[i]).screenPosition;
		xPoints.push_back(ofVec2f(t, screenPos.x));
		yPoints.push_back(ofVec2f(t, screenPos.y));
	}
	
	interpolationScreenX = getSplineInterpolation(xPoints);
	interpolationScreenY = getSplineInterpolation(yPoints);
	
	shouldRegenerateScreenPositionInterpolation = false;
}

void MeshLine::generateXYInterpolation() const {
	std::vector<ofVec2f> xPoints, yPoints;
	xPoints.reserve(vertIndices.size());
	yPoints.reserve(vertIndices.size());
	
	for (int i = 0; i < vertIndices.size(); i++) {
		float t = (float)i / (vertIndices.size() - 1);
		ofVec2f xy = mesh->getVert(vertIndices[i]).xy;
		xPoints.push_back(ofVec2f(t, xy.x));
		yPoints.push_back(ofVec2f(t, xy.y));
	}
	
	interpolationX = getSplineInterpolation(xPoints);
	interpolationY = getSplineInterpolation(yPoints);
	
	shouldRegenerateXYInterpolation = false;
}

ofVec2f MeshLine::getInterpolatedScreenPosition(float t) const {
	if (shouldRegenerateScreenPositionInterpolation) {
		generateScreenPositionInterpolation();
	}
	return ofVec2f(interpolationScreenX(t), interpolationScreenY(t));
}

MeshVert MeshLine::getInterpolatedMeshVert(float t) const {
	if (shouldRegenerateXYInterpolation) {
		generateXYInterpolation();
	}
	return MeshVert(getInterpolatedScreenPosition(t), ofVec2f(interpolationX(t), interpolationY(t)));
}

bool MeshLine::setAsDirty() {
	shouldRegenerateScreenPositionInterpolation = true;
	shouldRegenerateXYInterpolation = true;
}

void MeshLine::draw(int smooth) {
	if (shouldRegenerateScreenPositionInterpolation) generateScreenPositionInterpolation();
	
	if (smooth <= 1) {
		for (int i = 0; i < vertIndices.size() - 1; i++) {
			ofLine(mesh->getVert(vertIndices[i]).screenPosition, mesh->getVert(vertIndices[i+1]).screenPosition);
		}
	} else {
		int num = smooth * (vertIndices.size() - 1);
		float dt = 1.0f / num;
		ofVec2f p = getInterpolatedScreenPosition(0.0f);
		
		int i = 0;
		for (float t = dt; i < num; t += dt, i++) {
			ofVec2f p2 = getInterpolatedScreenPosition(t);
			ofLine(p, p2);
			p = p2;
		}
	}
}


MeshVert::MeshVert(const ofVec2f& screenPosition, const PolarCoords& polarCoords)
:screenPosition(screenPosition), xy(polarCoords.xy())
{
	
}

MeshVert::MeshVert(const ofVec2f& screenPosition, const ofVec2f& xy)
: screenPosition(screenPosition), xy(xy)
{
	
}

Mesh::Mesh() {
}

Mesh::~Mesh() {
}

MeshVert Mesh::getVert(int index) const {
	return verts[index];
}

ofVec2f Mesh::moveVert(int index, ofVec2f screenPosition) {
	verts[index].screenPosition = screenPosition;
	
	for (int i = 0; i < lines.size(); i++) {
		if (std::find(lines[i].vertIndices.begin(), lines[i].vertIndices.end(), index) != lines[i].vertIndices.end()) {
			lines[i].setAsDirty();
		}
	}
}

void Mesh::drawLines(int smooth) {
	for (int i = 0; i < lines.size(); i++) {
		lines[i].draw(smooth);
	}
}

int Mesh::getVertsNum() const {
	return verts.size();
}

const MeshLine& Mesh::getLine(int index) const {
	return lines[index];
}

int Mesh::getLinesNum() const {
	return lines.size();
}

ofVec2f Mesh::convertPolarCoordsToScreenPosition(const PolarCoords& pc, bool* error) const {
	ofVec2f xy = pc.xy();
	
	*error = false;
	
	for (int i = 0; i < triIndices.size(); i += 3) {
		if (isInsideTriangle(xy, verts[triIndices[i]].xy, verts[triIndices[i+1]].xy, verts[triIndices[i+2]].xy)) {
			return triangleToScreenPosition(xy, verts[triIndices[i]], verts[triIndices[i+1]], verts[triIndices[i+2]], error);
		}
	}
	
	for (int i = 0; i < quadIndices.size(); i += 4) {
		const MeshVert& v0 = verts[quadIndices[i]];
		const MeshVert& v1 = verts[quadIndices[i+1]];
		const MeshVert& v2 = verts[quadIndices[i+2]];
		const MeshVert& v3 = verts[quadIndices[i+3]];
		if (isInsideTriangle(xy, v0.xy, v1.xy, v2.xy)) {
			return triangleToScreenPosition(xy, v0, v1, v2, error);
		}
		if (isInsideTriangle(xy, v0.xy, v2.xy, v3.xy)) {
			return triangleToScreenPosition(xy, v0, v2, v3, error);
		}
	}
	
	// cannot find screen position
	*error = true;
	return ofVec2f();
}

std::string Mesh::getCompositionString() const {
	std::stringstream ss;
	for (int i = 0; i < verts.size(); i++) {
		ss << verts[i].screenPosition.x << "," << verts[i].screenPosition.y << " ";
	}
	return ss.str();
}

bool Mesh::parseCompositionString(const std::string& str) {
	int i = 0;
	int n = 0;
	std::stringstream buf;
	
	while (i < str.size() && n < verts.size()) {
		ofVec2f screenPosition;
		
		// read float value
		while ( (('0' <= str[i] && str[i] <= '9') || str[i] == '.') && i < str.size() ) {
			buf << str[i];
			i++;
		}
		buf >> screenPosition.x;
		buf.str("");
		buf.clear();

		// skip space and comma
		while ( (str[i] == ',' || isspace(str[i]) ) && i < str.size() ) {
			i++;
		}
		
		//
		while ( (('0' <= str[i] && str[i] <= '9') || str[i] == '.') && i < str.size() ) {
			buf << str[i];
			i++;
		}
		buf >> screenPosition.y;
		buf.str("");
		buf.clear();
		
		// skip space
		while ( isspace(str[i]) && i < str.size() ) {
			i++;
		}
		
		verts[n].screenPosition = screenPosition;
		n++;
	}
	
	if (n != verts.size()) {
		return true;
	}
	
	return false;
}


QuarterSphereMesh::QuarterSphereMesh() {
	
}

QuarterSphereMesh::QuarterSphereMesh(float centerTheta, int h_div, int v_div)
: horizontalDivision(h_div), verticalDivision(v_div)
{
	verts.reserve(verticalDivision * (horizontalDivision + 1) + 1);
	
	// polar vert
	verts.push_back(MeshVert(
		ofVec2f(0.5f, 0.0f),
		PolarCoords(0.0f, M_PI_2)
	));
	
	for (int i = 0; i < verticalDivision; i++) {
		for (int j = 0; j < horizontalDivision + 1; j++) {
			ofVec2f screenPosition = ofVec2f(0.1f + (float)j / (horizontalDivision+1), (float)(i+1) / (verticalDivision+1));
			float theta = normalizeRad(centerTheta + ((float)j / horizontalDivision * 2.0f - 1.0f) * M_PI_2);
			float phi = M_PI_2 - M_PI_2 * (float)(i+1) / verticalDivision;
			verts.push_back(MeshVert(screenPosition, PolarCoords(theta, phi)));
		}
	}
	
	generateLines();
	generateFaces();
}

void QuarterSphereMesh::generateLines() {
	lines.clear();
	lines.reserve(1 + (horizontalDivision - 1) + verticalDivision);
	
	// top line
	{
		MeshLine topLine(this);
		topLine.vertIndices.reserve(verticalDivision*2 + 1);
		for (int i = verticalDivision - 1; i >= 0; i--) {
			topLine.vertIndices << i * (horizontalDivision + 1) + 1;
		}
		topLine.vertIndices << 0;
		for (int i = 0; i < verticalDivision; i++) {
			topLine.vertIndices << (1 + i) * (horizontalDivision + 1);
		}
		lines << topLine;
	}
	
	// horizontal lines
	{
		for (int i = 0; i < verticalDivision; i++) {
			MeshLine line(this);
			line.vertIndices.reserve(horizontalDivision+1);
			for (int j = 0; j < horizontalDivision+1; j++) {
				line.vertIndices << i * (horizontalDivision+1) + j + 1;
			}
			lines << line;
		}
	}
	
	// vertical lines
	{
		for (int i = 0; i < horizontalDivision - 1; i++) {
			MeshLine line(this);
			line.vertIndices.reserve(verticalDivision+1);
			line.vertIndices << 0;
			for (int j = 0; j < verticalDivision; j++) {
				line.vertIndices << j * (horizontalDivision+1) + 2 + i;
			}
			lines << line;
		}
	}
}

void QuarterSphereMesh::generateFaces() {
	triIndices.reserve(horizontalDivision * 3);
	for (int i = 0; i < horizontalDivision; i++) {
		triIndices << 0 << i+1 << i+2;
	}
	
	quadIndices.reserve(horizontalDivision * (verticalDivision - 1) * 4);
	for (int i = 0; i < verticalDivision-1; i++) {
		for (int j = 0; j < horizontalDivision; j++) {
			int lefttop = i * (horizontalDivision+1) + j + 1;
			int leftbottom = (i+1) * (horizontalDivision+1) + j + 1;
			quadIndices << lefttop << lefttop+1 << leftbottom+1 << leftbottom;
		}
	}
}

ofPtr<QuarterSphereMesh> QuarterSphereMesh::createDivision() {
	ofPtr<QuarterSphereMesh> mesh = ofPtr<QuarterSphereMesh>(new QuarterSphereMesh());
	
	mesh->horizontalDivision = horizontalDivision * 2;
	mesh->verticalDivision = verticalDivision * 2;
	
	mesh->verts.resize(mesh->verticalDivision * (mesh->horizontalDivision + 1) + 1);

	// add existing verts
	mesh->verts[0] = verts[0];
	for (int j = 0; j < verticalDivision; j++) {
		for (int i = 0; i <= horizontalDivision; i++) {
			int from = i + j * (horizontalDivision + 1) + 1;
			int to = i*2 + j * (mesh->horizontalDivision + 1) * 2 + mesh->horizontalDivision + 2;
			mesh->verts[to] = verts[from];
		}
	}
	
	// add division points of the top line
	{
		float dt = 1.0f / (lines[0].vertIndices.size() - 1);
		int i = verticalDivision - 1;
		for (float t = dt * 0.5f; t < 0.5f; t += dt, i--) {
			int to = i * (mesh->horizontalDivision+1)*2 + 1;
			mesh->verts[to] = lines[0].getInterpolatedMeshVert(t);
		}
		i = 0;
		for (float t = 0.5f + dt * 0.5f; t < 1.0f; t += dt, i++) {
			int to = i * (mesh->horizontalDivision+1)*2 + mesh->horizontalDivision + 1;
			mesh->verts[to] = lines[0].getInterpolatedMeshVert(t);
		}
	}
	
	// add cross points of additional horizontal lines and existing vertical lines
	for (int j = 0; j < horizontalDivision - 1; j++) {
		MeshLine& vline = lines[verticalDivision+1 + j];
		float dt = 1.0f / verticalDivision;
		float t = dt * 0.5f;
		for (int i = 0; i < verticalDivision; i++, t += dt) {
			int to = j*2 + 3 + i * (mesh->horizontalDivision+1)*2;
			mesh->verts[to] = vline.getInterpolatedMeshVert(t);
		}
	}
	
	// add cross points of additional vertical lines and existing horizontal lines
	for (int j = 0; j < verticalDivision; j++) {
		MeshLine& hline = lines[1 + j];
		float dt = 1.0f / horizontalDivision;
		float t = dt * 0.5f;
		for (int i = 0; i < horizontalDivision; i++, t += dt) {
			int to = i*2 + mesh->horizontalDivision+3 + j * (mesh->horizontalDivision+1) * 2;
			mesh->verts[to] = hline.getInterpolatedMeshVert(t);
		}
	}
	
	// add interpolated mid points
	for (int j = 0; j < verticalDivision; j++) {
		for (int i = 0; i < horizontalDivision; i++) {
			int pos = 2 + i * 2 + j * (mesh->horizontalDivision + 1) * 2;
			int top = MAX(pos - (mesh->horizontalDivision+1), 0);
			int bottom = pos + mesh->horizontalDivision + 1;
			ofVec2f scrPos = (mesh->verts[pos-1].screenPosition + mesh->verts[pos+1].screenPosition + mesh->verts[top].screenPosition + mesh->verts[bottom].screenPosition) * 0.25f;
			ofVec2f xy = (mesh->verts[pos-1].xy + mesh->verts[pos+1].xy + mesh->verts[top].xy + mesh->verts[bottom].xy) * 0.25f;
			mesh->verts[pos] = MeshVert(scrPos, xy);
		}
	}
	
	//
	mesh->generateLines();
	mesh->generateFaces();
	
	return mesh;
}

ofPtr<QuarterSphereMesh> QuarterSphereMesh::create(float centerTheta, int horizontalDivsion, int verticalDivision) {
	return ofPtr<QuarterSphereMesh>(new QuarterSphereMesh(centerTheta, horizontalDivsion, verticalDivision));
}

const MeshLine& QuarterSphereMesh::getTopLine() const {
	return lines[0];
}

const MeshLine& QuarterSphereMesh::getNthHorizontalLine(int i) const {
	assert(1 + i < lines.size());
	return lines[1 + i];
}

const MeshLine& QuarterSphereMesh::getNthVerticalLine(int i) const {
	assert(verticalDivision + i + 1 < lines.size());
	return lines[verticalDivision + i + 1];
}

bool QuarterSphereMesh::loadCompositionString(const std::string& str) {
	bool error = parseCompositionString(str);
	if (!error) {
		
		for (int i = 0; i < lines.size(); i++) {
			lines[i].setAsDirty();
		}
		
		generateLines();
		generateFaces();
	}
	return error;
}