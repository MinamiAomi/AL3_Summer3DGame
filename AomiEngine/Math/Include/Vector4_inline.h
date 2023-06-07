#pragma once
#include "Vector4.h"
#include "Vector3.h"

inline Vector4::Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
inline Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
inline Vector4::Vector4(float xyzw) : x(xyzw), y(xyzw), z(xyzw), w(xyzw) {}
inline Vector4::Vector4(const Vector3& xyz, float w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
inline float& Vector4::operator[](size_t i) { 
	assert(i < 4);
	return (&x)[i]; 
}
inline const float& Vector4::operator[](size_t i) const { 
	assert(i < 4);
	return (&x)[i]; 
}
inline Vector4::operator Vector3() const {
	return { x, y, z };
}
inline Vector4 operator+(const Vector4& v) { return v; }
inline Vector4 operator-(const Vector4& v) { return { -v.x, -v.y, -v.z, -v.w }; }
inline Vector4 operator+(const Vector4& v1, const Vector4& v2) { return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w }; }
inline Vector4 operator-(const Vector4& v1, const Vector4& v2) { return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w }; }
inline Vector4 operator*(const Vector4& v, float s) { return { v.x * s, v.y * s, v.z * s, v.w * s }; }
inline Vector4 operator*(float s, const Vector4& v) { return { s * v.x, s * v.y, s * v.z, s * v.w }; }
inline Vector4& operator+=(Vector4& v1, const Vector4& v2) {
	v1 = v1 + v2;
	return v1;
}
inline Vector4& operator-=(Vector4& v1, const Vector4& v2) {
	v1 = v1 - v2;
	return v1;
}
inline Vector4& operator*=(Vector4& v, float s) {
	v = v * s;
	return v;
}
inline bool operator==(const Vector4& v1, const Vector4& v2) { return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w; }
inline bool operator!=(const Vector4& v1, const Vector4& v2) { return !(v1 == v2); }
inline Vector4& Vector4::SetXYZ(const Vector3& xyz) {
	x = xyz.x;
	y = xyz.y;
	z = xyz.z;
	return *this;
}
inline Vector3 Vector4::GetXYZ() const { return { x, y, z }; }
inline float Vector4::LengthSquare() const { return x * x + y * y + z * z + w * w; }
inline float Vector4::Length() const { return std::sqrt(LengthSquare()); }
inline Vector4 Vector4::Normalized() const {
	assert(*this != zero);
	return *this * (1.0f / Length());
}

inline float Vector4::Dot(const Vector4& lhs, const Vector4& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w; }
inline Vector4 Vector4::Scale(const Vector4& lhs, const Vector4& rhs) { return { lhs.x * rhs.x,lhs.y * rhs.y,lhs.z * rhs.z,lhs.w * rhs.w }; }
inline Vector4 Vector4::Project(const Vector4& base, const Vector4& direction) {
	Vector4 normDir = direction.Normalized();
	return Dot(base, normDir) * normDir;
}
inline Vector4 Vector4::Min(const Vector4& lhs, const Vector4& rhs) {
	return { std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z), std::min(lhs.w, rhs.w) };
}
inline Vector4 Vector4::Max(const Vector4& lhs, const Vector4& rhs) {
	return { std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z), std::max(lhs.w, rhs.w) };
}
inline Vector4 Vector4::Lerp(float t, const Vector4& start, const Vector4& end) { return start + t * (end - start); }
