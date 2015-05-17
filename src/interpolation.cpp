#include "interpolation.h"

#include <cassert>

namespace {
	bool compareVec2f(const ofVec2f& a, const ofVec2f& b) {
		return a.x < b.x;
	}
	bool compareQuat(const ofxDome::QuaternionAnchor& a, const ofxDome::QuaternionAnchor& b) {
		return a.x < b.x;
	}
}

namespace ofxDome {

	interpolation_func_t getSplineInterpolation(const std::vector<ofVec2f>& points, int k) {
		assert(k == 3);
		
		std::vector<ofVec2f> sorted = points;	// copy;
		std::sort(sorted.begin(), sorted.end(), &compareVec2f);
		
		int n = points.size() - 1;
		std::vector<float> h(n-1);
		std::vector<float> b(n), c(n+1), d(n), l(n+1), u(n), z(n+1), alpha(n);
		for (int i = 1; i <= n - 1; i++) {
			h[i] = sorted[i+1].x - sorted[i].x;
		}
		h[0] = h[1];
		for (int i = 1; i <= n-1; i++) {
			alpha[i] = 3.0f / h[i] * (sorted[i+1].y - sorted[i].y) - 3.0f / h[i-1] * (sorted[i].y - sorted[i-1].y);
		}
		l[0] = u[0] = z[0] = 0.0f;
		for (int i = 2; i <= n-1; i++) {
			l[i] = 2 * (sorted[i+1].x - sorted[i-1].x) - h[i-1] * u[i-1];
			u[i] = h[i] / l[i];
			z[i] = (alpha[i] - h[i-1]*z[i-1]) / l[i];
		}
		l[n] = 1;
		z[n] = c[n] = 0;
		for (int i = n-1; i >= 0; i--) {
			c[i] = z[i] - u[i]*c[i+1];
			b[i] = (sorted[i+1].y - sorted[i].y) / h[i] - h[i] * (c[i+1] + 2*c[i]) / 3.0f;
			d[i] = (c[i+1] - c[i]) / (3 * h[i]);
		}
		
		struct {
			std::vector<ofVec2f> sorted;
			std::vector<float> b, c, d;
			int n;
			float operator()(float given_x) const {
				int j;
				if (given_x < sorted[1].x) {
					j = 0;
				} else if (given_x > sorted[n].x) {
					j = n;
				} else {
					for (int i = 0; i <= n - 1; i++) {
						if (sorted[i].x <= given_x && given_x <= sorted[i+1].x) {
							j = i;
							break;
						}
					}
				}
				return sorted[j].y + b[j] * (given_x - sorted[j].x) + c[j] * pow(given_x - sorted[j].x, 2.0f) + d[j] * pow(given_x - sorted[j].x, 3.0f);
			}
		} returnValue;
		returnValue.sorted = sorted;
		returnValue.b = b;
		returnValue.c = c;
		returnValue.d = d;
		returnValue.n = n;
		
		return Function_Float<float>(returnValue);
	}

	interpolation_func_t getLagrangeInterpolation(const std::vector<ofVec2f>& points) {
		
		struct {
			std::vector<ofVec2f> points;
			float operator()(float x) const {
				for (int i = 0; i < points.size(); i++) {
					if (std::abs(x - points[i].x) < 0.0001f) {
						return points[i].y;
					}
				}
				
				float sigma = 0.0f;
				for (int j = 0; j < points.size(); j++) {
					float pi = points[j].y;
					for (int i = 0; i < points.size(); i++) {
						if (i == j) continue;
						pi *= (x - points[i].x) / (points[j].x - points[i].x);
					}
					sigma += pi;
				}
				return sigma;
			}
		} returnValue;
		
		returnValue.points = points;
		return Function_Float<float>(returnValue);
	}

	interpolation_func_t getLinearInterpolation(const std::vector<ofVec2f>& points) {
		std::vector<ofVec2f> sorted_points = points;	// copy;
		std::sort(sorted_points.begin(), sorted_points.end(), &compareVec2f);
		
		struct {
			std::vector<ofVec2f> sorted_points;
			float operator()(float x) {
				int low = 0, high = 1;
				for (int i = 0; i < sorted_points.size() - 1; i++) {
					if (sorted_points[i].x <= x && x <= sorted_points[i+1].x) {
						low = i;
						high = i+1;
						break;
					}
				}
				if (sorted_points[sorted_points.size()-1].x < x) {
					low = sorted_points.size() - 2;
					high = sorted_points.size() - 1;
				}
				float level = (x - sorted_points[low].x) / (sorted_points[high].x - sorted_points[low].x);
				return sorted_points[high].y * level + sorted_points[low].y * (1.0f - level);
			};
		} returnValue;
		
		returnValue.sorted_points = sorted_points;
		return Function_Float<float>(returnValue);
	}
    
    
	Function_Float<ofQuaternion> getLinearInterpolation(const std::vector<QuaternionAnchor>& points) {
		std::vector<QuaternionAnchor> sorted_points = points;	// copy;
		std::sort(sorted_points.begin(), sorted_points.end(), &compareQuat);
        
        struct {
			std::vector<QuaternionAnchor> sorted_points;
            ofQuaternion operator()(float x) {
				int low = 0, high = 1;
				for (int i = 0; i < sorted_points.size() - 1; i++) {
					if (sorted_points[i].x <= x && x <= sorted_points[i+1].x) {
						low = i;
						high = i+1;
						break;
					}
				}
				if (sorted_points[sorted_points.size()-1].x < x) {
					low = sorted_points.size() - 2;
					high = sorted_points.size() - 1;
				}
				float level = (x - sorted_points[low].x) / (sorted_points[high].x - sorted_points[low].x);
                
                ofQuaternion quat;
                quat.slerp(level, sorted_points[low].y, sorted_points[high].y);
                return quat;
            }
        } returnValue;
        returnValue.sorted_points = sorted_points;
        
        return Function_Float<ofQuaternion>(returnValue);
    }
}