#pragma once

#include <cassert>

#include "ofVec3f.h"
#include "ofQuaternion.h"

#include "Types.h"

namespace ofxDome {
	
	float normalizeRad(float rad);
	
	class PolarCoords {
	protected:
		mutable ofVec3f _direction;
		mutable ofQuaternion _quaternion;
		mutable float _theta, _phi;
		mutable bool _hasDirection, _hasQuaternion, _hasThetaPhi;
	public:
		PolarCoords() {
			_hasDirection = _hasQuaternion = _hasThetaPhi = false;
		};
		
		PolarCoords(float theta, float phi)
		: _theta(theta), _phi(phi), _hasDirection(false), _hasQuaternion(false), _hasThetaPhi(true)
		{
		}
		
		explicit PolarCoords(const ofQuaternion& quat)
		: _quaternion(quat), _hasDirection(false), _hasQuaternion(true), _hasThetaPhi(false)
		{
			
		}
		
		explicit PolarCoords(const ofVec3f& dir)
		: _direction(dir), _hasDirection(true), _hasQuaternion(false), _hasThetaPhi(false)
		{
			
		}
		
		ofVec2f xy() const {
			assert(_hasThetaPhi);
			return ofVec2f(cos(_theta), sin(_theta)) * (MATH_PI() * 0.5f - _phi);
		}
		
		ofQuaternion quaternion() const {
			if (!_hasQuaternion) {
				assert(_hasThetaPhi);
				_quaternion = ofQuaternion(90.0f - _phi * RAD_TO_DEG, ofVec3f(1.0f, 0.0f, 0.0f)) * ofQuaternion(_theta * RAD_TO_DEG, ofVec3f(0.0f, 1.0f, 0.0f));
				_hasQuaternion = true;
			}
			return _quaternion;
		}
		
		ofVec3f direction() const {
			if (!_hasDirection) {
				_direction = quaternion() * ofVec3f(0.0f, 1.0f, 0.0f);
				_hasDirection = true;
			}
			return _direction;
		}
		
		float theta() const {
			assert(_hasThetaPhi);
			return _theta;
		}
		
		float phi() const {
			assert(_hasThetaPhi);
			return _phi;
			
		}
	};
}