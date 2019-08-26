#ifndef __TOOL_HPP__
#define __TOOL_HPP__
#include "cocos2d.h"
#include <cstdint>

#define SAME_SIGNS( a, b )	\
		(((std::int64_t) ((std::uint64_t) a ^ (std::uint64_t) b)) >= 0 )
namespace mtl {
	float linearThreshHold(float minT, float maxT, float minV, float valueT, float slope);

	int random(int min, int max);

	cocos2d::Vec2 rotateZ(float angle, const cocos2d::Vec2 & vec);
	
	template<typename T> T clamp(T min, T max, T value) {
		if (value < min) {
			return min;
		}
		if (value > max) {
			return max;
		}
		return value;
	}

	template<typename T, typename V>
	inline V lerp(T min, T max, T value, V outValue) {
		if (value >= max) {
			return outValue;
		}
		else if (value <= min) {
			return 0;
		}
		return (V)(outValue * (value - min ) / (max - min));
	}

	template <typename T> 
	int sign(T val) {
		return (T(0) < val) - (val < T(0));
	}

	bool isVecOutRange(float min, float max, const cocos2d::Vec2 & vec);
	bool isVecOutRange(float min, float max, const cocos2d::Vec3 & vec);
	cocos2d::Quaternion getQuaternion(const cocos2d::Vec3 & a, const cocos2d::Vec3 & b );
	bool isInRange(float min, float max, float value);
	cocos2d::Vec2 findPointOfRectLine(const cocos2d::Rect &rect, const cocos2d::Vec2 &p1, const cocos2d::Vec2 &p2, float paddingX, float padidngY);
	bool isLineIntersect(const cocos2d::Vec2 &p1, const cocos2d::Vec2 &p2, const cocos2d::Vec2 &p3, const cocos2d::Vec2 &p4, cocos2d::Vec2 &result);
}

#endif