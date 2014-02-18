#include "testApp.h"



bool ImageOrVideo::load(const std::string& path) {
    if (image.loadImage(path)) {
        isImage = true;
        return true;
    }
    if (video.loadMovie(path)) {
        video.setLoopState(OF_LOOP_NORMAL);
        isImage = false;
        return true;
    }
    return false;
}

void ImageOrVideo::update() {
    if (!isImage) {
        video.update();
    }
}

void ImageOrVideo::draw(float width, float height) {
    if (isImage) {
        image.draw(0.0f, 0.0f, width, height);
    } else {
        video.draw(0.0f, 0.0f, width, height);
    }
}


int ImageOrVideo::getWidth() const {
    if (isImage) {
        return image.width;
    } else {
        return video.width;
    }
}

int ImageOrVideo::getHeight() const {
    if (isImage) {
        return image.height;
    } else {
        return video.height;
    }
}

void ImageOrVideo::play() {
    if (!isImage) video.play();
}

//--------------------------------------------------------------
void testApp::setup() {
    ofSetVerticalSync(true);
    
    gui.setup();
    gui.setPosition(0.0, ofGetWindowHeight() * 0.5f);
    gui.add(panoramaButton.setup("Panorama", true));
    panoramaButton.addListener(this, &testApp::onPanoramaButtonPressed);
    gui.add(polarButton.setup("Polar", false));
    polarButton.addListener(this, &testApp::onPolarButtonPressed);
    gui.add(cubeButton.setup("Cube", false));
    cubeButton.addListener(this, &testApp::onCubeButtonPressed);
    
	dome.setScreenDivision(3, 1);
	dome.setCoordinateSystemPanorama(2048, 512);
	dome.setup();
	
	dome.loadMeshCompositionString(ofBufferFromFile("mesh.txt").getText());
    
    ofAddListener(ofEvents().draw, this, &testApp::drawGui);
}

//--------------------------------------------------------------
void testApp::update() {
    for (int i = 0; i < dome.countFbos(); i++) {
        images[i].update();
    }
}

void testApp::onPanoramaButtonPressed() {
    ImageOrVideo image;
    
    ofFileDialogResult result = ofSystemLoadDialog("Choose a panoramic image or video");
    if (!result.bSuccess || !image.load(result.filePath)) {
        ofSystemAlertDialog("Cannot read file");
        return;
    }
    
    float upAngle = 4.0f * 90.0f * image.getHeight() / image.getWidth();
    do {
        upAngle = ofFromString<float>(ofSystemTextBoxDialog("Specify maximum angle of elevation (0-90)", ofToString(upAngle)));
    } while (upAngle < 0.0f || upAngle > 90.0f);
    
    images[0] = image;
    images[0].play();

    dome.setCoordinateSystemPanorama(image.getWidth(), image.getHeight(), ofDegToRad(upAngle));
}

void testApp::onPolarButtonPressed() {
    ImageOrVideo image;
    
    ofFileDialogResult result = ofSystemLoadDialog("Choose a polar image or video");
    if (!result.bSuccess || !image.load(result.filePath)) {
        ofSystemAlertDialog("Cannot read file");
        return;
    }
    
    float upAngle = 0.0f;
    do {
        upAngle = ofFromString<float>(ofSystemTextBoxDialog("Specify minimum angle of elevation (0-90)", ofToString(upAngle)));
    } while (upAngle < 0.0f || upAngle > 90.0f);
    
    images[0] = image;
    images[0].play();
    
    dome.setCoordinateSystemPolar(MAX(image.getWidth(), image.getHeight()), ofDegToRad(upAngle));
}

void testApp::onCubeButtonPressed() {
    std::string messages[] = {
        "Choose a top-side image or video",
        "Choose a left-side image or video",
        "Choose a front-side image or video",
        "Choose a right-side image or video",
        "Choose a back-side image or video"
    };
    
    ImageOrVideo imgs[5];
    int maxSize = 0;
    
    for (int i = 0; i < 5; i++) {
        ofFileDialogResult result = ofSystemLoadDialog(messages[i]);
        if (!result.bSuccess || !imgs[i].load(result.filePath)) {
            ofSystemAlertDialog("Cannot read file");
            return;
        }
        maxSize = MAX(MAX(maxSize, imgs[i].getWidth()), imgs[i].getHeight());
    }
    
    dome.setCoordinateSystemCube(maxSize);
    
    for (int i = 0; i < 5; i++) {
        images[i] = imgs[i];
        images[i].play();
    }
}

//--------------------------------------------------------------
void testApp::draw() {
    for (int i = 0; i < dome.countFbos(); i++) {
        ofFbo& fbo = dome.beginFbo(i);
        images[i].draw(fbo.getWidth(), fbo.getHeight());
        dome.endFbo(i);
    }
}

//--------------------------------------------------------------
void testApp::drawGui(ofEventArgs& e) {
    ofViewport(0.0f, 0.0f, ofGetWindowWidth(), ofGetWindowHeight());
    gui.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed( int key ) {
	switch (key) {
        case 'l':
            dome.setMeshVisiblity(!dome.getMeshVisiblity());
            break;
        case 'f':
            ofToggleFullscreen();
            break;
	}
}

void testApp::mouseReleased(int x, int y, int button) {
	ofBuffer buffer(dome.getMeshCompositionString());
	ofBufferToFile("mesh.txt", buffer);
}