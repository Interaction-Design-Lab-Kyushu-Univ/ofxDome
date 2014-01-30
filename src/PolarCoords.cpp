#include "PolarCoords.h"

namespace ofxDome {

	float normalizeRad(float rad) {
		while (rad <= - M_PI) {
			rad += M_PI * 2.0f;
		}
		while (rad > M_PI) {
			rad -= M_PI * 2.0f;
		}
		return rad;
	}

}