#pragma once
#include "Quaternion.h"
#include "Matrix4x4.h"

inline Quaternion::Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
inline Quaternion::Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
inline float& Quaternion::operator[](size_t i) { 
	assert(i < 4);
	return (&x)[i]; 
}
inline const float& Quaternion::operator[](size_t i) const { 
	assert(i < 4);
	return (&x)[i]; 
}
inline Quaternion operator+(const Quaternion& v1, const Quaternion& v2) { return Quaternion{ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w }; }
inline Quaternion operator*(const Quaternion& v, float s) { return Quaternion{ v.x * s, v.y * s, v.z * s, v.w * s }; }
inline Quaternion operator*(float s, const Quaternion& v) { return Quaternion{ s * v.x, s * v.y, s * v.z, s * v.w }; }
inline Vector3 operator*(const Quaternion& lhs, const Vector3& rhs) {
	Vector3 qv = lhs.GetXYZ();
	return rhs + 2.0f * Vector3::Cross(qv, Vector3::Cross(qv, rhs) + lhs.w * rhs);
}
inline Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs) {
	return Quaternion{
		lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z,
		lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x,
		lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z
	};
}
inline Quaternion& operator+=(Quaternion& v1, const Quaternion& v2) {
	v1 = v1 + v2;
	return v1;
}
inline Quaternion& operator*=(Quaternion& v, float s) {
	v = v * s;
	return v;
}

inline Quaternion& operator*=(Quaternion lhs, const Quaternion& rhs) {
	lhs = lhs * rhs;
	return lhs;
}
inline bool operator==(const Quaternion& v1, const Quaternion& v2) { return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w; }
inline bool operator!=(const Quaternion& v1, const Quaternion& v2) { return !(v1 == v2); }
inline Quaternion& Quaternion::SetEulerAngle(const Vector3& eulerAngle) {
	Vector3 s = Vector3(std::sin(eulerAngle.x * 0.5f), std::sin(eulerAngle.y * 0.5f), std::sin(eulerAngle.z * 0.5f));
	Vector3 c = Vector3(std::cos(eulerAngle.x * 0.5f), std::cos(eulerAngle.y * 0.5f), std::cos(eulerAngle.z * 0.5f));
	x = s.x * c.y * c.z - c.x * s.y * s.z;
	y = c.x * s.y * c.z + s.x * c.y * s.z;
	z = c.x * c.y * s.z - s.x * s.y * c.z;
	w = c.x * c.y * c.z + s.x * s.y * s.z;
	return *this;
}
inline Vector3 Quaternion::GetEulerAngle() const {
	Vector3 euler{};
	euler.x = std::atan2(2.0f * (w * x + y * z), 1.0f - 2.0f * (x * x + y * y));
	euler.y = std::asin(2.0f * (w * y - z * x));
	euler.z = std::atan2(2.0f * (w * z + x * y), 1.0f - 2.0f * (y * y + z * z));
	return euler;
}
inline Quaternion Quaternion::Normalized() const { return *this * (1.0f / std::sqrt(Dot(*this, *this))); }
inline Vector3 Quaternion::GetXYZ() const { return { x,y,z }; }
inline Quaternion& Quaternion::SetAngle(float angle) {
	Vector3 tmpAxis = GetAxis();
	*this = MakeFromAngleAxis(angle, tmpAxis);
	return *this;
}
inline float Quaternion::GetAngle() const {
	return std::acos(w) * 2.0f;
}
inline Quaternion& Quaternion::SetAxis(const Vector3& axis) {
	float tmpAngle = GetAngle();
	*this = MakeFromAngleAxis(tmpAngle, axis);
	return *this;
}
inline Vector3 Quaternion::GetAxis() const {
	return GetXYZ() * (1.0f / std::sin(std::acos(w)));
}
inline Quaternion Quaternion::GetConjugate() {
	return Quaternion{ -x,-y,-z,w };
}
inline Quaternion Quaternion::GetInverse() {
	return GetConjugate() * (1.0f / std::sqrt(Dot(*this, *this)));
}
inline float Quaternion::Dot(const Quaternion& lhs, const Quaternion& rhs) {
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}
inline Quaternion Quaternion::Lerp(float t, const Quaternion& start, const Quaternion& end) {
	return Quaternion{
		start.x + t * (end.x - start.x),
		start.y + t * (end.y - start.y),
		start.z + t * (end.z - start.z),
		start.w + t * (end.w - start.w)	};
}
inline Quaternion Quaternion::MakeFromAngleAxis(float angle, const Vector3& axis) {
	float sin2 = std::sin(angle * 0.5f);
	return Quaternion{
		axis.x * sin2,
		axis.y * sin2,
		axis.z * sin2,
		std::cos(angle * 0.5f)
	};
}
inline Quaternion Quaternion::MakeFromEulerAngle(const Vector3& euler) {
	// ピッチ ヨー ロールの順
	Vector3 s = Vector3(std::sin(euler.x * 0.5f), std::sin(euler.y * 0.5f), std::sin(euler.z * 0.5f));
	Vector3 c = Vector3(std::cos(euler.x * 0.5f), std::cos(euler.y * 0.5f), std::cos(euler.z * 0.5f));
	return Quaternion{
		s.x * c.y * c.z - c.x * s.y * s.z,
		c.x * s.y * c.z + s.x * c.y * s.z,
		c.x * c.y * s.z - s.x * s.y * c.z,
		c.x * c.y * c.z + s.x * s.y * s.z
	};
}
inline Quaternion Quaternion::MakeForXAxis(float angle) {
	return Quaternion(std::sin(angle / 2.0f), 0.0f, 0.0f, std::cos(angle / 2.0f));
}
inline Quaternion Quaternion::MakeForYAxis(float angle) {
	return Quaternion(0.0f, std::sin(angle / 2.0f), 0.0f, std::cos(angle / 2.0f));
}
inline Quaternion Quaternion::MakeForZAxis(float angle) {
	return Quaternion(0.0f, 0.0f, std::sin(angle / 2.0f), std::cos(angle / 2.0f));
}
inline Quaternion Quaternion::MakeFromTwoVector(const Vector3& v1, const Vector3& v2) {
	Vector3 tmpAxis = Vector3::Cross(v1, v2).Normalized();
	float tmpAngle = std::acos(Vector3::Dot(v1, v2));
	return MakeFromAngleAxis(tmpAngle, tmpAxis);
}
inline Quaternion Quaternion::MakeFromOrthonormal(const Vector3& x, const Vector3& y, const Vector3& z) {
	float trace = x.x + y.y + z.z;

	if (trace > 0.0f) {
		float s = std::sqrt(trace + 1.0f) * 0.5f;
		Quaternion result{};
		result.w = s;
		s = 0.25f / s;
		result.x = (y.z - z.y) * s;
		result.y = (z.x - x.z) * s;
		result.z = (x.y - y.x) * s;
		return result;
	}
	else if (x.x > y.y && x.x > z.z) {
		float s = std::sqrt(1.0f + x.x - y.y - z.z) * 0.5f;
		Quaternion result{};
		result.x = s;
		s = 0.25f / s;
		result.y = (x.y + y.x) * s;
		result.z = (z.x + x.z) * s;
		result.w = (y.z - z.y) * s;
		return result;
	}
	else if (y.y > z.z) {
		float s = std::sqrt(1.0f - x.x + y.y - z.z) * 0.5f;
		Quaternion result{};
		result.y = s;
		s = 0.25f / s;
		result.x = (x.y + y.x) * s;
		result.z = (y.z + z.y) * s;
		result.w = (z.x - x.z) * s;
		return result;
	}
	Quaternion result{};
	float s = std::sqrt(1.0f - x.x - y.y + z.z) * 0.5f;
	result.z = s;
	s = 0.25f / s;
	result.x = (z.x + x.z) * s;
	result.y = (y.z + z.y) * s;
	result.w = (x.y - y.x) * s;
	return result;
}
inline Quaternion Quaternion::MakeLookRotation(const Vector3& direction, const Vector3& up) {
	Vector3 z = direction.Normalized();
	Vector3 x = Vector3::Cross(up, z).Normalized();
	Vector3 y = Vector3::Cross(z, x);
	return MakeFromOrthonormal(x, y, z);
}
inline Quaternion Quaternion::MakeFromMatrix(const Matrix4x4& m) {
	Vector3 x{ m.m[0][0],m.m[0][1],m.m[0][2] };
	Vector3 y{ m.m[1][0],m.m[1][1],m.m[1][2] };
	Vector3 z{ m.m[2][0],m.m[2][1],m.m[2][2] };
	return MakeFromOrthonormal(x, y, z);
}
