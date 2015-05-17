#include "PolarCoords.h"

namespace ofxDome {

	float normalizeRad(float rad) {
		while (rad <= - MATH_PI()) {
			rad += MATH_PI() * 2.0f;
		}
		while (rad > MATH_PI()) {
			rad -= MATH_PI() * 2.0f;
		}
		return rad;
	}

}