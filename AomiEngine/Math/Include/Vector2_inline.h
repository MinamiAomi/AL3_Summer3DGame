#pragma once
#include "Vector2.h"
#include "Vector3.h"

inline Vector2::Vector2() : x(0.0f), y(0.0f) {}
inline Vector2::Vector2(float x, float y) : x(x), y(y) {}
inline Vector2::Vector2(float xy) : x(xy), y(xy) {}
inline float& Vector2::operator[](size_t i) { 
	assert(i < 2);
	return (&x)[i]; 
}
inline const float& Vector2::operator[](size_t i) const { 
	assert(i < 2);
	return (&x)[i]; 
}
inline Vector2::operator Vector3() const { return { x, y, 0.0f }; }
inline Vector2 operator+(const Vector2& v) { return v; }
inline Vector2 operator-(const Vector2& v) { return { -v.x, -v.y }; }
inline Vector2 operator+(const Vector2& v1, const Vector2& v2) { return { v1.x + v2.x, v1.y + v2.y }; }
inline Vector2 operator-(const Vector2& v1, const Vector2& v2) { return { v1.x - v2.x, v1.y - v2.y }; }
inline Vector2 operator*(const Vector2& v, float s) { return { v.x * s, v.y * s }; }
inline Vector2 operator*(float s, const Vector2& v) { return { s * v.x, s * v.y }; }
inline Vector2& operator+=(Vector2& v1, const Vector2& v2) {
	v1 = v1 + v2;
	return v1;
}
inline Vector2& operator-=(Vector2& v1, const Vector2& v2) {
	v1 = v1 - v2;
	return v1;
}
inline Vector2& operator*=(Vector2& v, float s) {
	v = v * s;
	return v;
}
inline bool operator==(const Vector2& v1, const Vector2& v2) { return v1.x == v2.x && v1.y == v2.y; }
inline bool operator!=(const Vector2& v1, const Vector2& v2) { return !(v1 == v2); }
inline float Vector2::LengthSquare() const { return x * x + y * y; }
inline float Vector2::Length() const { return std::sqrt(LengthSquare()); }
inline Vector2 Vector2::Normalized() const { 
	assert(*this != zero);
	return *this * (1.0f / Length()); 
}
inline float Vector2::Angle(const Vector2& v1, const Vector2& v2) { return std::acos(Dot(v1.Normalized(), v2.Normalized())); }
inline float Vector2::SignedAngle(const Vector2& from, const Vector2& to) {
	float angle = Angle(from, to);
	return Cross(from, to) >= 0.0f ? angle : -angle;
}
inline float Vector2::Distance(const Vector2& v1, const Vector2& v2) { return (v2 - v1).Length(); }
inline float Vector2::Dot(const Vector2& v1, const Vector2& v2) { return v1.x * v2.x + v1.y * v2.y; }
inline float Vector2::Cross(const Vector2& v1, const Vector2& v2) { return v1.x * v2.y - v1.y * v2.x; }
inline Vector2 Vector2::Perpendicular(const Vector2& v) { return { -v.y, v.x }; }
inline Vector2 Vector2::Scale(const Vector2& v1, const Vector2& v2) { return { v1.x * v2.x, v1.y * v2.y }; }
inline Vector2 Vector2::Project(const Vector2& v1, const Vector2& v2) {
	Vector2 normV2 = v2.Normalized();
	return Dot(v1, normV2) * normV2;
}
inline Vector2 Vector2::Reflecte(const Vector2& v, const Vector2& n) { 
	Vector2 normN = n.Normalized();
	return Dot(normN, -v) * 2.0f * normN + v; 
}
inline Vector2 Vector2::Min(const Vector2& v1, const Vector2& v2) { return { std::min(v1.x, v2.x), std::min(v1.y,v2.y) }; }
inline Vector2 Vector2::Max(const Vector2& v1, const Vector2& v2) { return { std::max(v1.x, v2.x), std::max(v1.y,v2.y) }; }
inline Vector2 Vector2::Lerp(float t, const Vector2& start, const Vector2& end) { return start + t * (end - start); }

