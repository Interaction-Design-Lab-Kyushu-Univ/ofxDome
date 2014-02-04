#pragma once

#include <ofVec2f.h>
#include <vector>
#include "Types.h"
#include <ofMain.h>

namespace ofxDome {
	
	template <typename T> class Function_Float {
	protected:
		ofPtr<void> capture;
		T(*func)(void *, float);
	public:
		Function_Float() : func(NULL) {};
		
		template <typename M> explicit Function_Float(const M& m)  {
			capture = ofPtr<void>(new M(m));
			class Temp {
			public:
				static T call(void* p, float t) {
					M* mp = (M*)p;
					return (*mp)(t);
				}
			};
			func = &Temp::call;
		};
		
		Function_Float(const Function_Float& source)
		: capture(source.capture), func(source.func)
		{};
		
		T operator()(float t) {
			return func(capture.get(), t);
		};
	};
    
    class QuaternionAnchor {
    public:
        float x;
        ofQuaternion y;
        QuaternionAnchor() {};
        QuaternionAnchor(float x, const ofQuaternion& y) : x(x), y(y) {};
    };
	
	typedef Function_Float<float> interpolation_func_t;

	interpolation_func_t getSplineInterpolation(const std::vector<ofVec2f>& points, int n = 3);
	interpolation_func_t getLagrangeInterpolation(const std::vector<ofVec2f>& points);
	interpolation_func_t getLinearInterpolation(const std::vector<ofVec2f>& points);
    
	Function_Float<ofQuaternion> getLinearInterpolation(const std::vector<QuaternionAnchor>& points);
}