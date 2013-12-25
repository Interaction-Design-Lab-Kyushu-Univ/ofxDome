#pragma once

#include <ofVec2f.h>
#include <vector>
#include "Types.h"
#include <ofMain.h>

namespace ofxDome {
	
	class InterpolationFunc {
	protected:
		ofPtr<void> capture;
		float(*func)(void *, float);
	public:
		InterpolationFunc() : func(NULL) {};
		
		template <typename M> explicit InterpolationFunc(const M& m)  {
			capture = ofPtr<void>(new M(m));
			class Temp {
			public:
				static float call(void* p, float t) {
					M* mp = (M*)p;
					return (*mp)(t);
				}
			};
			func = &Temp::call;
		};
		
		InterpolationFunc(const InterpolationFunc& source)
		: capture(source.capture), func(source.func)
		{};
		
		float operator()(float t) {
			return func(capture.get(), t);
		};
	};
	
	typedef InterpolationFunc interpolation_func_t;

	interpolation_func_t getSplineInterpolation(const std::vector<ofVec2f>& points, int n = 3);
	interpolation_func_t getLagrangeInterpolation(const std::vector<ofVec2f>& points);
	interpolation_func_t getLinearInterpolation(const std::vector<ofVec2f>& points);
}