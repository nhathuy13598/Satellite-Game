#include "Tool.hpp"
#include <random>
#include <cmath>
#include "../GameScene.h"

USING_NS_CC;

float mtl::linearThreshHold(float minT, float maxT, float minV, float valueT, float slope)
{
	if (valueT <= minT) {
		return minV;
	}
	if (valueT >= maxT) {
		return minV + (maxT - minT) * slope;
	}
	return minV + (valueT - minT) * slope;
}

int mtl::random(int min, int max)
{
	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_int_distribution<std::mt19937::result_type> uni(min, max); // guaranteed unbiased
	return uni(rng);
}

//angle in degree
cocos2d::Vec2 mtl::rotateZ(float angle, const cocos2d::Vec2 & vec)
{
	angle = CC_DEGREES_TO_RADIANS(angle);
	float sinValue = std::sin(angle);
	float cosValue = std::cos(angle);
	return cocos2d::Vec2{vec.x * cosValue - vec.y * sinValue, 
	vec.y * cosValue + vec.x * sinValue};
}

bool mtl::isVecOutRange(float min, float max, const cocos2d::Vec2 & vec)
{
	return vec.x < min || vec.x > max || vec.y < min || vec.y > max;
}

bool mtl::isVecOutRange(float min, float max, const cocos2d::Vec3 & vec)
{
	return vec.x < min || vec.x > max || vec.y < min || vec.y > max || vec.z < min || vec.z > max;
}

cocos2d::Quaternion mtl::getQuaternion(const cocos2d::Vec3 & a, const cocos2d::Vec3 & b)
{
	float dot = a.dot(b);
	cocos2d::Vec3 normal;
	cocos2d::Vec3::cross(a, b, &normal);
	cocos2d::Quaternion quat{ normal.x, normal.y, normal.z, dot + 1 };
	quat.normalize();
	return quat;
}

bool mtl::isInRange(float min, float max, float value)
{
	return value >= min && value <= max;
}

cocos2d::Vec2 mtl::findPointOfRectLine(const cocos2d::Rect & rect, const cocos2d::Vec2 & p1, const cocos2d::Vec2 & p2, float paddingX, float paddingY)
{
	Vec2 topLeft{ rect.getMinX() + paddingX, rect.getMaxY() - paddingY };
	Vec2 botLeft{ rect.getMinX() + paddingX, rect.getMinY() + paddingY };
	Vec2 botRight{ rect.getMaxX() - paddingX, rect.getMinY() + paddingY };
	Vec2 topRight{ rect.getMaxX() - paddingX, rect.getMaxY() - paddingY };
	Vec2 point;
	if (isLineIntersect(topLeft, botLeft, p1, p2, point)) {
		return point;
	}
	else if (isLineIntersect(botLeft, botRight, p1, p2, point)) {
		return point;
	}
	else if (isLineIntersect(botRight, topRight, p1, p2, point)) {
		return point;
	}
	else if (isLineIntersect(topRight, topLeft, p1, p2, point)) {
		return point;
	}
	return point;
}

bool mtl::isLineIntersect(const cocos2d::Vec2 & p1, const cocos2d::Vec2 & p2, const cocos2d::Vec2 & p3, const cocos2d::Vec2 & p4, cocos2d::Vec2 &result)
{
	Vec2 d21 = p2 - p1;
	d21.normalize();
	Vec2 d21Normal{ d21.y, -d21.x };
	double c21 = -d21Normal.x * p1.x - d21Normal.y * p1.y;
	double r1 = d21Normal.x * p3.x + d21Normal.y * p3.y + c21;
	double r2 = d21Normal.x * p4.x + d21Normal.y * p4.y + c21;
	if (r1 != 0 && r2 != 0 && r1 * r2 >= 0) {
		return false;
	}

	Vec2 d43 = p4 - p3;
	d43.normalize();
	Vec2 d43Normal{ d43.y, -d43.x };
	double c43 = -d43Normal.x * p3.x - d43Normal.y * p3.y;
	r1 = d43Normal.x * p1.x + d43Normal.y * p1.y + c43;
	r2 = d43Normal.x * p2.x + d43Normal.y * p2.y + c43;
	if (r1 != 0 && r2 != 0 && r1 * r2 >= 0) {
		return false;
	}

	double denom = d21Normal.x * d43Normal.y - d21Normal.y * d43Normal.x;
	if (denom == 0) {
		return false;
	}
	double dx = c43 * d21Normal.y - c21 * d43Normal.y;
	double dy = d43Normal.x * c21 - d21Normal.x * c43;
	result.x = dx / denom;
	result.y = dy / denom;
	return true;
}