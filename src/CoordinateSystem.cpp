#include "CoordinateSystem.h"
#include "Types.h"

using namespace ofxDome;

std::size_t CoordinateSystem::UVTexIdHash::operator () (CoordinateSystem::UVTexId value) const {
	return value.uv.x * 1.0f + value.uv.y * 100.0f + value.textuireId * 10000.0f;
}

bool CoordinateSystem::UVTexId::operator == (const UVTexId& other) const {
	return uv == other.uv && textuireId == other.textuireId;
}


CoordinateSystem::CoordinateSystem()
{
	cache = map_t(UV_DIVISION * UV_DIVISION * getTexturesNum());
}

PolarCoords CoordinateSystem::convertUVFast(const ofVec2f& uv, int textureId) const {
	UVTexId uvtex = {uv, textureId};
	
	map_t::ConstIterator itr = cache.find(uvtex);
	
	if (itr != cache.end()) {
		return itr->second;
	} else {
		PolarCoords pc = convertUV(uv, textureId);
		cache.insert(std::pair<UVTexId, PolarCoords>(uvtex, pc));
		return pc;
	}
}


CoordinateSystemPolar::CoordinateSystemPolar(float minPhi, float textureSize)
: minPhi(minPhi), textureSize(textureSize)
{
	
}

PolarCoords CoordinateSystemPolar::convertUV(const ofVec2f& uv, int textureId) const {
	ofVec2f xy = uv * 2.0f - ofVec2f(1.0f, 1.0f);
	
	float distance = xy.length();
	float phi = MATH_PI() * 0.5f * (1.0f - distance) + minPhi * distance;
	
	float theta = atan2(xy.y, xy.x);
	return PolarCoords(theta, phi);
}

ofVec2f CoordinateSystemPolar::getMaxTexCoords(int textureId) const {
	return ofVec2f(textureSize, textureSize);
}

int CoordinateSystemPolar::getTexturesNum() const {
	return 1;
}



CoordinateSystemPanorama::CoordinateSystemPanorama(ofVec2f textureSize, float maxPhi)
: textureSize(textureSize), maxPhi(maxPhi)
{
	
}

PolarCoords CoordinateSystemPanorama::convertUV(const ofVec2f& uv, int textureId) const {
	float theta = (uv.x * 2.0f - 1.0f) * MATH_PI();
	float phi = maxPhi - uv.y * maxPhi;
	return PolarCoords(theta, phi);
}

ofVec2f CoordinateSystemPanorama::getMaxTexCoords(int textureId) const {
	return textureSize;
}

int CoordinateSystemPanorama::getTexturesNum() const {
	return 1;
}



CoordinateSystemCube::CoordinateSystemCube(float textureSize)
: textureSize(textureSize)
{
	
}

PolarCoords CoordinateSystemCube::convertUV(const ofVec2f& uv, int textureId) const {
	ofVec3f vec;
	switch (textureId) {
	case CUBE_TEXTURE_TOP:
		vec = ofVec3f(uv.x * 2.0f - 1.0f, 1.0f, - 1 * (uv.y * 2.0f - 1.0f) );
		break;
	case CUBE_TEXTURE_LEFT:
		vec = ofVec3f(1.0f, (uv.y * 2.0f - 1.0f) * -1, uv.x * 2.0f - 1.0f);
		break;
	case CUBE_TEXTURE_FRONT:
		vec = ofVec3f(uv.x * 2.0f - 1.0f, (uv.y * 2.0f - 1.0f) * -1, -1.0f);
		break;
	case CUBE_TEXTURE_RIGHT:
		vec = ofVec3f(-1.0f, (uv.y * 2.0f - 1.0f) * -1, 1.0f - uv.x * 2.0f);
		break;
	case CUBE_TEXTURE_BACK:
		vec = ofVec3f(1.0f - uv.x * 2.0f, (uv.y * 2.0f - 1.0f) * -1, 1.0f);
		break;
	}
	
	vec.normalize();
	
	float theta = atan2(vec.z, vec.x);
	float phi = asin(vec.y);
	
	return PolarCoords(theta, phi);
}

ofVec2f CoordinateSystemCube::getMaxTexCoords(int textureId) const {
	return ofVec2f(textureSize, textureSize);
}

int CoordinateSystemCube::getTexturesNum() const {
	return 5;
}
