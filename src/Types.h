#pragma once

#include "PolarCoords.h"

namespace ofxDome {
	typedef enum {
		COORDINATE_SYSTEM_POLAR,
		COORDINATE_SYSTEM_PANORAMA,
		COORDINATE_SYSTEM_CUBE,
	} ECoordinateSystem;
	
	typedef enum {
		CUBE_TEXTURE_TOP,
		CUBE_TEXTURE_LEFT,
		CUBE_TEXTURE_FRONT,
		CUBE_TEXTURE_RIGHT,
		CUBE_TEXTURE_BACK,
		CUBE_TEXTURES_NUM
	} ECubeTextureIndex;
	
	static const int UV_DIVISION = 100;
}