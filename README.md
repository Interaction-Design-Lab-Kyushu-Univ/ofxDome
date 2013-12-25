#ofxDome
ofxDome is an extension for making projection mapping onto a dome.
A specification for the dome and other hardwares are available in Taiyo Kogyo's website: .
Current version is based on version 0.8.0.

##License
ofxDome is under the MIT License.
Please see License.txt for details.

##Setup
Add all src/* files into your project and set include paths.

Set screen size and window mode in main.cpp appropliately.
ofxDome requires at least 3 screens; 2 for projector and 1 for controls.
When you tile screens horizontally on your OS preference, you have to make 3x width fullscreen window.

##Configuration

Add
`#include "ofxDome/ofxDome.h"`

Add an instance into your testApp class
`class testApp : public ofBaseApp {
	ofxDome::ofxDome dome;
	// omit
};`

Configure your dome projection inside setup().

Set the number of tiled screens by using
`dome.setScreenDivision(x, y)`

You can choose 3 coordinate systems and size of texture(s).
`dome.setCoordinateSystemPanorama(w, h);`
	[Illustration]

`dome.setCoordinateSystemPolar(wh);`
	[Illustration]

`dome.setCoordinateSystemCube(wh);`
	[Illustration]

Finally, call `dome.setup()` to finish configuration.

##Drawing
Draw your own texture inside draw().
Your code must be between dome.beginFbo and dome.endFbo.

###Panorama or Polar Coords
`dome.beginFbo()
// your code here
dome.endFbo()`

###Cube Coords
You must prepare 5 textures.

`dome.beginFbo(ofxDome::CUBE_TEXTURE_TOP);
dome.endFbo();
dome.beginFbo(ofxDome::CUBE_TEXTURE_LEFT);
dome.endFbo();
dome.beginFbo(ofxDome::CUBE_TEXTURE_FRONT);
dome.endFbo();
dome.beginFbo(ofxDome::CUBE_TEXTURE_RIGHT);
dome.endFbo();
dome.beginFbo(ofxDome::CUBE_TEXTURE_BACK);
dome.endFbo();`

##Other Methods
All other methods should be called after `dome.setup()`.

`void loadMeshCompositionString(const std::string& string)`
`std::string getMeshCompositionString() const`
	read or write csv-like string that represents mesh composition.

`void setMeshVisibility(bool visible)`
`bool getMeshVisibility() const`
	Show or Hide lines on the projector screens.