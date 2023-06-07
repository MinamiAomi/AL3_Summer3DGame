#pragma once
#include "Vector3.h"
#include "Vector2.h"

inline Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
inline Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
inline Vector3::Vector3(float xyz) : x(xyz), y(xyz), z(xyz) {}
inline Vector3::Vector3(const Vector2& xy, float z) : x(xy.x), y(xy.y), z(z) {}
inline float& Vector3::operator[](size_t i) { 
	assert(i < 3);
	return (&x)[i]; 
}
inline const float& Vector3::operator[](size_t i) const { 
	assert(i < 3);
	return (&x)[i]; 
}
inline Vector3 operator+(const Vector3& v) { return v; }
inline Vector3 operator-(const Vector3& v) { return { -v.x, -v.y, -v.z }; }
inline Vector3 operator+(const Vector3& v1, const Vector3& v2) { return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z }; }
inline Vector3 operator-(const Vector3& v1, const Vector3& v2) { return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z }; }
inline Vector3 operator*(const Vector3& v, float s) { return { v.x * s, v.y * s, v.z * s }; }
inline Vector3 operator*(float s, const Vector3& v) { return { s * v.x, s * v.y, s * v.z }; }
inline Vector3& operator+=(Vector3& v1, const Vector3& v2) {
	v1 = v1 + v2;
	return v1;
}
inline Vector3& operator-=(Vector3& v1, const Vector3& v2) {
	v1 = v1 - v2;
	return v1;
}
inline Vector3& operator*=(Vector3& v, float s) {
	v = v * s;
	return v;
}
inline bool operator==(const Vector3& v1, const Vector3& v2) { return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z; }
inline bool operator!=(const Vector3& v1, const Vector3& v2) { return !(v1 == v2); }
inline Vector3& Vector3::SetXY(const Vector2& xy) {
	x = xy.x;
	y = xy.y;
	return *this;
}
inline Vector2 Vector3::GetXY() const { return { x, y }; }
inline Vector3& Vector3::SetXZ(const Vector2& xz) {
	x = xz.x;
	z = xz.y;
	return *this;
}
inline Vector2 Vector3::GetXZ() const { return { x, z }; }
inline Vector3& Vector3::SetYZ(const Vector2& yz) {
	y = yz.x;
	z = yz.y;
	return *this;
}
inline Vector2 Vector3::GetYZ() const { return { y, z }; }
inline float Vector3::LengthSquare() const { return x * x + y * y + z * z; }
inline float Vector3::Length() const { return std::sqrt(LengthSquare()); }
inline Vector3 Vector3::Normalized() const { 
	assert(*this != zero);
	return *this * (1.0f / Length()); 
}
inline float Vector3::Angle(const Vector3& from, const Vector3& to) { return std::acos(Dot(from.Normalized(), to.Normalized())); }
inline float Vector3::SignedAngle(const Vector3& from, const Vector3& to, const Vector3& axis) { return Dot(Cross(from, to), axis) < 0.0f ? Angle(from, to) : -Angle(from, to); }
inline float Vector3::Distance(const Vector3& v1, const Vector3& v2) { return (v2 - v1).Length(); }
inline float Vector3::Dot(const Vector3& lhs, const Vector3& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }
inline Vector3 Vector3::Cross(const Vector3& lhs, const Vector3& rhs) {
	return {
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x };
}
inline Vector3 Vector3::Scale(const Vector3& lhs, const Vector3& rhs) { return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z }; }
inline Vector3 Vector3::Project(const Vector3& base, const Vector3& direction) {
	Vector3 normDir = direction.Normalized();
	return Dot(base, normDir) * normDir;
}
inline Vector3 Vector3::ProjectOnPlane(const Vector3& base, const Vector3& planeNormal) { 
	Vector3 normNormal = planeNormal.Normalized();
	return base - normNormal * Dot(base, normNormal);
}
inline Vector3 Vector3::Reflecte(const Vector3& direction, const Vector3& normal) { 
	Vector3 normNormal = normal.Normalized();
	return Dot(normNormal, -direction) * 2.0f * normNormal + direction;
}
inline Vector3 Vector3::Min(const Vector3& lhs, const Vector3& rhs) { return { std::min(lhs.x,rhs.x), std::min(lhs.y,rhs.y), std::min(lhs.z,rhs.z) }; }
inline Vector3 Vector3::Max(const Vector3& lhs, const Vector3& rhs) { return { std::max(lhs.x,rhs.x), std::max(lhs.y,rhs.y), std::max(lhs.z,rhs.z) }; }
inline Vector3 Vector3::Lerp(float t, const Vector3& start, const Vector3& end) { return start + t * (end - start); }
