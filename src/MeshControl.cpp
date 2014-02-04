#include "MeshControl.h"

using namespace ofxDome;

namespace {
	const float CLICKABLE_POINT_RADIUS = 5.0f;
	const int LINE_SMOOTHNESS = 5;
	const float SELECTED_POINT_RADIUS = 6.0f;
	const float POINT_RADIUS = 3.0f;
}

MeshControl::MeshControl(ofPtr<QuarterSphereMesh> mesh, ofPtr<CoordinateSystem> cs, ofPtr<std::vector<ofFbo> > fbos)
: mesh(mesh), coordinateSystem(cs), fbos(fbos)
{
	dividedMesh = mesh->createDivision();
	gradation.reset(new QuarterSphereGradation(dividedMesh, 1.0f));
	
	for (int i = 0; i < cs->getTexturesNum(); i++) {
		textureWarps.push_back(ofPtr<TextureWarp>(new TextureWarp(cs, dividedMesh, i)));
		textureWarps.back()->generate();
	}
}

ofVec2f MeshControl::normalizePosition(const ofRectangle& rect, const ofVec2f& pos) const
{
	ofVec2f pos2 = pos - rect.getMin();
	ofVec2f clamped = ofVec2f(CLAMP(pos2.x, 0.0f, rect.getWidth()), CLAMP(pos2.y, 0.0f, rect.getHeight()));
	return ofVec2f(clamped.x / rect.getWidth(), clamped.y / rect.getHeight());
}

ofVec2f MeshControl::inverseNormalizedPosition(const ofRectangle& rect, const ofVec2f& pos) const
{
	return rect.getMin() + ofVec2f(rect.getWidth() * pos.x, rect.getHeight() * pos.y);
}

/* return true if a control point is clicked */
bool MeshControl::mousePressed(const ofRectangle& rect, const ofVec2f& pos) {
	for (int i = 0; i < mesh->getVertsNum(); i++) {
		if (inverseNormalizedPosition(rect, mesh->getVert(i).screenPosition).distanceSquared(pos) < CLICKABLE_POINT_RADIUS*CLICKABLE_POINT_RADIUS) {
			selectedPoint = i;
			isDragging = true;
			return true;
		}
	}

	selectedPoint = NOT_SELECTED;
	return false;
}

void MeshControl::mouseMoved(const ofRectangle& rect, const ofVec2f& pos) {
	if (isDragging) {
	} else {
		for (int i = 0; i < mesh->getVertsNum(); i++) {
			if (inverseNormalizedPosition(rect, mesh->getVert(i).screenPosition).distanceSquared(pos) < CLICKABLE_POINT_RADIUS*CLICKABLE_POINT_RADIUS) {
				selectedPoint = i;
				return;
			}
		}
		selectedPoint = NOT_SELECTED;
	}
}

void MeshControl::mouseDragged(const ofRectangle& rect, const ofVec2f& pos) {
	if (isDragging) {
		mesh->moveVert(selectedPoint, normalizePosition(rect, pos));
		notifyMeshUpdated();
		
	} else {
		selectedPoint = NOT_SELECTED;
	}
}

void MeshControl::mouseReleased(const ofRectangle& rect, const ofVec2f& pos) {
	selectedPoint = NOT_SELECTED;
	isDragging = false;
}

void MeshControl::draw(const ofRectangle& rect, bool drawLines) {
	
	ofPushMatrix();
	ofTranslate(rect.getMin());
	ofScale(rect.getWidth(), rect.getHeight());
	
	for (int i = 0; i < coordinateSystem->getTexturesNum(); i++) {
		ofTexture& texture = fbos->at(i).getTextureReference();
		texture.bind();
		textureWarps[i]->draw();
		texture.unbind();
	}
	
	gradation->draw();
	
	if (drawLines) {
		dividedMesh->drawLines(LINE_SMOOTHNESS);
	}
	ofPopMatrix();
	
	if (drawLines) {
		for (int i = 0; i < mesh->getVertsNum(); i++) {
			ofVec2f pos = inverseNormalizedPosition(rect, mesh->getVert(i).screenPosition);
			ofCircle(pos, i == selectedPoint? SELECTED_POINT_RADIUS : POINT_RADIUS);
		}
	}
}

void MeshControl::notifyMeshUpdated() {
	dividedMesh = mesh->createDivision();
	
	for (int i = 0; i < coordinateSystem->getTexturesNum(); i++) {
		textureWarps[i]->setMesh(dividedMesh);
		textureWarps[i]->generate();
	}
	
	gradation->setMesh(dividedMesh);
	gradation->generate();
}