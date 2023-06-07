#include "stdafx.h"
#include "Vector2.h"

const Vector2 Vector2::unitX = { 1.0f,0.0f };
const Vector2 Vector2::unitY = { 0.0f,1.0f };
const Vector2 Vector2::zero = { 0.0f,0.0f };
const Vector2 Vector2::one = { 1.0f,1.0f };
const Vector2 Vector2::right = { 1.0f,0.0f };
const Vector2 Vector2::left = { -1.0f,0.0f };
const Vector2 Vector2::up = { 0.0f,1.0f };
const Vector2 Vector2::down = { 0.0f,-1.0f };

Vector2 Vector2::Slerp(float t, const Vector2& start, const Vector2& end) {
	assert(start != zero);
	assert(end != zero);
	float lenStart = start.Length();
	float lenEnd = end.Length();
	Vector2 normStart = start * (1.0f / lenStart);
	Vector2 normEnd = end * (1.0f / lenEnd);
	
	float dot = Dot(normStart, normEnd);
	if (std::abs(dot) > 0.999f) {
		return Lerp(t, start, end);
	}
	float theta = std::acos(dot);
	float divSinTheta = 1.0f / std::sin(theta);
	float t1 = std::sin((1.0f - t) * theta) * divSinTheta;
	float t2 = std::sin(t * theta) * divSinTheta;
	float scale = lenStart + t * (lenEnd - lenStart);
	return (t1 * normStart + t2 * normEnd) * scale;
}
Vector2 Vector2::CatmullRomSpline(float t, const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3) {
	return
		0.5f * ((-p0 + 3.0f * p1 - 3.0f * p2 + p3) * std::pow(t, 3.0f)
			+ (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * std::pow(t, 2.0f)
			+ (-p0 + p2) * t
			+ 2.0f * p1);
}
Vector2 Vector2::QuadraticBezierCurve(float t, const Vector2& p0, const Vector2& p1, const Vector2& p2) {
	float s = 1.0f - t;
	return
		s * s * p0 +
		2.0f * t * s * p1 +
		t * t * p2;
}
Vector2 Vector2::CubicBezierCurve(float t, const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3) {
	float s = 1.0f - t;
	return
		(s * s * s * p0) +
		(3.0f * s * s * t * p1) +
		(3.0f * s * t * t * p2) +
		t * t * t * p3;
}
