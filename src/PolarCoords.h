#pragma once

#include "ofVec3f.h"
#include "ofQuaternion.h"

namespace ofxDome {
	
	float normalizeRad(float rad);
	
	class PolarCoords {
	public:
		float theta;
		float phi;
		
		PolarCoords() {};
		
		PolarCoords(float theta, float phi)
		: theta(theta), phi(phi)
		{
			
		}
		
		ofVec2f xy() const {
			return ofVec2f(cos(theta), sin(theta)) * (M_PI_2 - phi);
		}
		
		ofQuaternion quaternion() const {
			return ofQuaternion(theta * RAD_TO_DEG, ofVec3f(0.0f, 1.0f, 0.0f)) * ofQuaternion(phi * RAD_TO_DEG, ofVec3f(1.0f, 0.0f, 0.0f));
		}
		
		ofVec3f direction() const {
			return quaternion() * ofVec3f(1.0f, 0.0f, 0.0f);
		}
	};
}