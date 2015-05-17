#pragma once

#include "PolarCoords.h"
#include <Poco/HashMap.h>

namespace Poco {
}

namespace ofxDome {
	
	/**
	 * Coordinate System provides conversion between texCoords and PolarCoords
	 * Coordinate System may require multiple textures
	 */
	class CoordinateSystem {
		class UVTexId {
		public:
			ofVec2f uv;
			int textuireId;
			bool operator == (const UVTexId& other) const;
		};
		
		struct UVTexIdHash {
			std::size_t operator () (CoordinateSystem::UVTexId value) const;
		};
		
		typedef Poco::HashMap<UVTexId, PolarCoords, UVTexIdHash> map_t;
		mutable map_t cache;
		
	public:
		CoordinateSystem();
		virtual ~CoordinateSystem() {};
		
	protected:
		/**
		 * @param uv texCoords normalized between 0 and 1
		 * @param textureId index of texture
		 */
		virtual PolarCoords convertUV(const ofVec2f& uv, int textureId) const = 0;
		
	public:
		/**
		 * convert uv coords into polar coords
		 * the result is cached
		 */
		PolarCoords convertUVFast(const ofVec2f& uv, int textureId) const;
		
		/**
		 * @return Size of each textures
		 */
		virtual ofVec2f getMaxTexCoords(int textureId) const = 0;
		
		/**
		 * @return number of textures required
		 */
		virtual int getTexturesNum() const { return 1; };
	};
	
	class CoordinateSystemPolar : public CoordinateSystem {
		float minPhi;
		float textureSize;
	protected:
		PolarCoords convertUV(const ofVec2f& uv, int textureId) const;
	public:
		CoordinateSystemPolar(float minPhi, float textureSize);
		ofVec2f getMaxTexCoords(int textureId) const;
		int getTexturesNum() const;
	};
	
	class CoordinateSystemPanorama : public CoordinateSystem {
		ofVec2f textureSize;
		float maxPhi;
	protected:
		PolarCoords convertUV(const ofVec2f& uv, int textureId) const;
	public:
		CoordinateSystemPanorama(ofVec2f textureSize, float maxPhi = MATH_PI() * 0.5f);
		ofVec2f getMaxTexCoords(int textureId) const;
		int getTexturesNum() const;
	};
	
	class CoordinateSystemCube : public CoordinateSystem {
		float textureSize;
	protected:
		PolarCoords convertUV(const ofVec2f& uv, int textureId) const;
	public:
		CoordinateSystemCube(float textureSize);
		ofVec2f getMaxTexCoords(int textureId) const;
		int getTexturesNum() const;
	};
}