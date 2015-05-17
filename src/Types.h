#pragma once

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
	
	inline float MATH_PI() {
		return 3.145926535897932f;
	}

	inline float OVERWRAP_RADIAN() {
		return 10.0f / 180.0f * MATH_PI();
	}
}