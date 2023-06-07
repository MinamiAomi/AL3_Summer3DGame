#include "Vector3.h"

const Vector3 Vector3::zero = { 0.0f, 0.0f, 0.0f };
const Vector3 Vector3::unitX = { 1.0f, 0.0f, 0.0f };
const Vector3 Vector3::unitY = { 0.0f, 1.0f, 0.0f };
const Vector3 Vector3::unitZ = { 0.0f, 0.0f, 1.0f };
const Vector3 Vector3::one = { 1.0f, 1.0f, 1.0f };
const Vector3 Vector3::right = { 1.0f, 0.0f, 0.0f };
const Vector3 Vector3::left = { -1.0f, 0.0f, 0.0f };
const Vector3 Vector3::up = { 0.0f, 1.0f, 0.0f };
const Vector3 Vector3::down = { 0.0f, -1.0f, 0.0f };
const Vector3 Vector3::forward = { 0.0f, 0.0f, 1.0f };
const Vector3 Vector3::back = { 0.0f, 0.0f, -1.0f };

inline Vector3 Vector3::Slerp(float t, const Vector3& start, const Vector3& end) {
	assert(start != zero);
	assert(end != zero);
	float lenStart = start.Length();
	float lenEnd = end.Length();
	Vector3 normStart = start * (1.0f / lenStart);
	Vector3 normEnd = end * (1.0f / lenEnd);

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

Vector3 Vector3::CatmullRomSpline(float t, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3) {
	return
		0.5f * ((-p0 + 3.0f * p1 - 3.0f * p2 + p3) * std::pow(t, 3.0f)
			+ (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * std::pow(t, 2.0f)
			+ (-p0 + p2) * t
			+ 2.0f * p1);
}

Vector3 Vector3::QuadraticBezierCurve(float t, const Vector3& p0, const Vector3& p1, const Vector3& p2) {
	float s = 1.0f - t;
	return
		s * s * p0 +
		2.0f * t * s * p1 +
		t * t * p2;
}

Vector3 Vector3::CubicBezierCurve(float t, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3) {
	float s = 1.0f - t;
	return
		(s * s * s * p0) +
		(3.0f * s * s * t * p1) +
		(3.0f * s * t * t * p2) +
		t * t * t * p3;
}
