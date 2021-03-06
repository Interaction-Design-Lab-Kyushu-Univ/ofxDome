#include "BaseApp.h"
#include "MasterScreen.h"
#include "ClientScreen.h"
#include "Mesh.h"
#include "MeshControl.h"

using namespace ofxDome;

BaseApp::BaseApp() {
	isUIEnabled = true;
	setScreenDivision(3, 1);
	fbos = ofPtr<std::vector<ofFbo> >(new std::vector<ofFbo>());
}

void BaseApp::setupInternal() {
	ofPtr<QuarterSphereMesh> qsmeshes[] = { QuarterSphereMesh::create(MATH_PI() * 0.5f), QuarterSphereMesh::create(- MATH_PI() * 0.5f) };
	meshControls[0] = ofPtr<MeshControl>(new MeshControl(qsmeshes[0], coordinateSystem, fbos));
	meshControls[1] = ofPtr<MeshControl>(new MeshControl(qsmeshes[1], coordinateSystem, fbos));
	meshes[0] = qsmeshes[0];
	meshes[1] = qsmeshes[1];
	
	masterScreen = ofPtr<MasterScreen>(new MasterScreen(
														ofPtr<MeshControl>(meshControls[0]),
														ofPtr<MeshControl>(meshControls[1]),
														fbos
														));
	
	screens.clear();
	screens.push_back(masterScreen);
	for (int i = 0; i <= 1; i++) {
		clientScreens[i] = ofPtr<ClientScreen>(new ClientScreen(i, meshControls[i]));
		clientScreens[i]->setInteractionEnabled(isUIEnabled);
		screens.push_back(clientScreens[i]);
	}
}

void BaseApp::setup() {
	MultiScreenApp::setup();
	assert(getTileX() * getTileY() >= 3);
	assert(coordinateSystem != NULL);
	
	setupInternal();
	setupScreens();
}

void BaseApp::setCoordinateSystem(ofPtr<CoordinateSystem> cs) {
	coordinateSystem = cs;
	fbos->resize(coordinateSystem->getTexturesNum());
	for (int i = 0; i < coordinateSystem->getTexturesNum(); i++) {
		fbos->at(i).allocate(coordinateSystem->getMaxTexCoords(i).x, coordinateSystem->getMaxTexCoords(i).y);
	}
	
	if (meshControls[0]) {
		string composition = getMeshCompositionString();
		setupInternal();
		setupScreens();
		loadMeshCompositionString(composition);
	}
}

void BaseApp::setMeshVisiblity(bool enabled) {
	isUIEnabled = enabled;
	clientScreens[0]->setInteractionEnabled(isUIEnabled);
	clientScreens[1]->setInteractionEnabled(isUIEnabled);
}

bool BaseApp::getMeshVisiblity() const {
	return isUIEnabled;
}

ofFbo& BaseApp::getFbo(int index) {
	return fbos->at(index);
}

int BaseApp::countFbos() const {
	return fbos->size();
}

std::string BaseApp::getMeshCompositionString() const {
	return meshes[0]->getCompositionString() + "/" + meshes[1]->getCompositionString();
}

bool BaseApp::loadMeshCompositionString(const std::string& string) {
	int i = 0;
	int splitPos = string.find('/');
	if (splitPos == string::npos) {
		return true;
	}
	
	bool error1 = meshes[0]->loadCompositionString(string.substr(0, splitPos));
	bool error2 = meshes[1]->loadCompositionString(string.substr(splitPos+1));
	
	if (!error1 && !error2) {
		meshControls[0]->notifyMeshUpdated();
		meshControls[1]->notifyMeshUpdated();
		return false;
	}
	
	return true;
}

void BaseApp::draw() {
	MultiScreenApp::draw();
}