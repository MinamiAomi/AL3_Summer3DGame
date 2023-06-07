#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"

inline Matrix4x4::Matrix4x4() {
	*this = identity;
}

inline Matrix4x4::Matrix4x4(
	float _00, float _01, float _02, float _03,
	float _10, float _11, float _12, float _13,
	float _20, float _21, float _22, float _23,
	float _30, float _31, float _32, float _33) {
	m[0][0] = _00, m[0][1] = _01, m[0][2] = _02, m[0][3] = _03;
	m[1][0] = _10, m[1][1] = _11, m[1][2] = _12, m[1][3] = _13;
	m[2][0] = _20, m[2][1] = _21, m[2][2] = _22, m[2][3] = _23;
	m[3][0] = _30, m[3][1] = _31, m[3][2] = _32, m[3][3] = _33;
}

inline Matrix4x4 operator*(const Matrix4x4& lhs, const Matrix4x4& rhs) {
	return {
			lhs.m[0][0] * rhs.m[0][0] + lhs.m[0][1] * rhs.m[1][0] + lhs.m[0][2] * rhs.m[2][0] + lhs.m[0][3] * rhs.m[3][0],
			lhs.m[0][0] * rhs.m[0][1] + lhs.m[0][1] * rhs.m[1][1] + lhs.m[0][2] * rhs.m[2][1] + lhs.m[0][3] * rhs.m[3][1],
			lhs.m[0][0] * rhs.m[0][2] + lhs.m[0][1] * rhs.m[1][2] + lhs.m[0][2] * rhs.m[2][2] + lhs.m[0][3] * rhs.m[3][2],
			lhs.m[0][0] * rhs.m[0][3] + lhs.m[0][1] * rhs.m[1][3] + lhs.m[0][2] * rhs.m[2][3] + lhs.m[0][3] * rhs.m[3][3],

			lhs.m[1][0] * rhs.m[0][0] + lhs.m[1][1] * rhs.m[1][0] + lhs.m[1][2] * rhs.m[2][0] + lhs.m[1][3] * rhs.m[3][0],
			lhs.m[1][0] * rhs.m[0][1] + lhs.m[1][1] * rhs.m[1][1] + lhs.m[1][2] * rhs.m[2][1] + lhs.m[1][3] * rhs.m[3][1],
			lhs.m[1][0] * rhs.m[0][2] + lhs.m[1][1] * rhs.m[1][2] + lhs.m[1][2] * rhs.m[2][2] + lhs.m[1][3] * rhs.m[3][2],
			lhs.m[1][0] * rhs.m[0][3] + lhs.m[1][1] * rhs.m[1][3] + lhs.m[1][2] * rhs.m[2][3] + lhs.m[1][3] * rhs.m[3][3],

			lhs.m[2][0] * rhs.m[0][0] + lhs.m[2][1] * rhs.m[1][0] + lhs.m[2][2] * rhs.m[2][0] + lhs.m[2][3] * rhs.m[3][0],
			lhs.m[2][0] * rhs.m[0][1] + lhs.m[2][1] * rhs.m[1][1] + lhs.m[2][2] * rhs.m[2][1] + lhs.m[2][3] * rhs.m[3][1],
			lhs.m[2][0] * rhs.m[0][2] + lhs.m[2][1] * rhs.m[1][2] + lhs.m[2][2] * rhs.m[2][2] + lhs.m[2][3] * rhs.m[3][2],
			lhs.m[2][0] * rhs.m[0][3] + lhs.m[2][1] * rhs.m[1][3] + lhs.m[2][2] * rhs.m[2][3] + lhs.m[2][3] * rhs.m[3][3],

			lhs.m[3][0] * rhs.m[0][0] + lhs.m[3][1] * rhs.m[1][0] + lhs.m[3][2] * rhs.m[2][0] + lhs.m[3][3] * rhs.m[3][0],
			lhs.m[3][0] * rhs.m[0][1] + lhs.m[3][1] * rhs.m[1][1] + lhs.m[3][2] * rhs.m[2][1] + lhs.m[3][3] * rhs.m[3][1],
			lhs.m[3][0] * rhs.m[0][2] + lhs.m[3][1] * rhs.m[1][2] + lhs.m[3][2] * rhs.m[2][2] + lhs.m[3][3] * rhs.m[3][2],
			lhs.m[3][0] * rhs.m[0][3] + lhs.m[3][1] * rhs.m[1][3] + lhs.m[3][2] * rhs.m[2][3] + lhs.m[3][3] * rhs.m[3][3] };
}

inline Matrix4x4& operator*=(Matrix4x4& lhs, const Matrix4x4& rhs) {
	lhs = lhs * rhs;
	return lhs;
}

inline Vector3 operator*(const Vector3& lhs, const Matrix4x4& rhs) {
	return {
			lhs.x * rhs.m[0][0] + lhs.y * rhs.m[1][0] + lhs.z * rhs.m[2][0] + rhs.m[3][0],
			lhs.x * rhs.m[0][1] + lhs.y * rhs.m[1][1] + lhs.z * rhs.m[2][1] + rhs.m[3][1],
			lhs.x * rhs.m[0][2] + lhs.y * rhs.m[1][2] + lhs.z * rhs.m[2][2] + rhs.m[3][2] };
}

inline Vector4 operator*(const Vector4& lhs, const Matrix4x4& rhs) {
	return {
		lhs.x * rhs.m[0][0] + lhs.y * rhs.m[1][0] + lhs.z * rhs.m[2][0] + lhs.w * rhs.m[3][0],
		lhs.x * rhs.m[0][1] + lhs.y * rhs.m[1][1] + lhs.z * rhs.m[2][1] + lhs.w * rhs.m[3][1],
		lhs.x * rhs.m[0][2] + lhs.y * rhs.m[1][2] + lhs.z * rhs.m[2][2] + lhs.w * rhs.m[3][2],
		lhs.x * rhs.m[0][3] + lhs.y * rhs.m[1][3] + lhs.z * rhs.m[2][3] + lhs.w * rhs.m[3][3] };
}

inline Matrix4x4 operator*(float lhs, const Matrix4x4& rhs) {
	return {
		lhs * rhs.m[0][0], lhs * rhs.m[0][1], lhs * rhs.m[0][2], lhs * rhs.m[0][3],
		lhs * rhs.m[1][0], lhs * rhs.m[1][1], lhs * rhs.m[1][2], lhs * rhs.m[1][3],
		lhs * rhs.m[2][0], lhs * rhs.m[2][1], lhs * rhs.m[2][2], lhs * rhs.m[2][3],
		lhs * rhs.m[3][0], lhs * rhs.m[3][1], lhs * rhs.m[3][2], lhs * rhs.m[3][3] };
}

inline Matrix4x4 operator*(const Matrix4x4& lhs, float rhs) {
	return {
		lhs.m[0][0] * rhs, lhs.m[0][1] * rhs, lhs.m[0][2] * rhs, lhs.m[0][3] * rhs,
		lhs.m[1][0] * rhs, lhs.m[1][1] * rhs, lhs.m[1][2] * rhs, lhs.m[1][3] * rhs,
		lhs.m[2][0] * rhs, lhs.m[2][1] * rhs, lhs.m[2][2] * rhs, lhs.m[2][3] * rhs,
		lhs.m[3][0] * rhs, lhs.m[3][1] * rhs, lhs.m[3][2] * rhs, lhs.m[3][3] * rhs };
}

inline Vector3 Matrix4x4::ApplyRotation(const Vector3& vector) const {
	return {
		vector.x *m[0][0] + vector.y * m[1][0] + vector.z * m[2][0],
		vector.x *m[0][1] + vector.y * m[1][1] + vector.z * m[2][1],
		vector.x *m[0][2] + vector.y * m[1][2] + vector.z * m[2][2] };
}

inline Vector3 Matrix4x4::ApplyTransformWDivide(const Vector3& vector) const {
	Vector3 result = {};
	result.x = vector.x * m[0][0] + vector.y * m[1][0] + vector.z * m[2][0] + m[3][0];
	result.y = vector.x * m[0][1] + vector.y * m[1][1] + vector.z * m[2][1] + m[3][1];
	result.z = vector.x * m[0][2] + vector.y * m[1][2] + vector.z * m[2][2] + m[3][2];
	float w = vector.x * m[0][3] + vector.y * m[1][3] + vector.z * m[2][3] + m[3][3];
	assert(w != 0.0f);
	result *= 1.0f / w;
	return result;
}

inline Matrix4x4& Matrix4x4::SetRow(size_t i, const Vector4& v) {
	assert(i < 4);
	m[i][0] = v.x, m[i][1] = v.y, m[i][2] = v.z, m[i][3] = v.w;
	return *this;
}

inline Vector4 Matrix4x4::GetRow(size_t i) const {
	assert(i < 4);
	return { m[i][0], m[i][1], m[i][2], m[i][3] };
}

inline Matrix4x4& Matrix4x4::SetColumn(size_t i, const Vector4& v) {
	assert(i < 4);
	m[0][i] = v.x, m[1][i] = v.y, m[2][i] = v.z, m[3][i] = v.w;
	return *this;
}

inline Vector4 Matrix4x4::GetColumn(size_t i) const {
	assert(i < 4);
	return { m[0][i], m[1][i], m[2][i], m[3][i] };
}

inline Vector3 Matrix4x4::GetXAxis() const {
	return { m[0][0], m[0][1], m[0][2] };
}

inline Vector3 Matrix4x4::GetYAxis() const {
	return { m[1][0], m[1][1], m[1][2] };
}

inline Vector3 Matrix4x4::GetZAxis() const {
	return { m[2][0], m[2][1], m[2][2] };
}

inline Vector3 Matrix4x4::GetTranslate() const {
	return { m[3][0], m[3][1], m[3][2] };
}

inline Matrix4x4 Matrix4x4::GetInverse() {
	return 1.0f / GetDeterminant() * GetAdjugate();
}

inline Matrix4x4 Matrix4x4::GetTranspose() {
	return {
		m[0][0], m[1][0], m[2][0], m[3][0],
		m[0][1], m[1][1], m[2][1], m[3][1],
		m[0][2], m[1][2], m[2][2], m[3][2],
		m[0][3], m[1][3], m[2][3], m[3][3] };
}

inline Matrix4x4 Matrix4x4::MakeScaling(const Vector3& scale) {
	return {
		scale.x, 0.0f, 0.0f, 0.0f,
		0.0f, scale.y, 0.0f, 0.0f,
		0.0f, 0.0f, scale.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
}

inline Matrix4x4 Matrix4x4::MakeRotationX(float rotate) {
	float c = std::cos(rotate);
	float s = std::sin(rotate);
	return {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, c, s, 0.0f,
		0.0f, -s, c, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
}

inline Matrix4x4 Matrix4x4::MakeRotationY(float rotate) {
	float s = std::sin(rotate);
	float c = std::cos(rotate);
	return {
		c,		0.0f,	-s,		0.0f,
		0.0f,	1.0f,	0.0f,	0.0f,
		s,		0.0f,	c,		0.0f,
		0.0f,	0.0f,	0.0f,	1.0f };
}

inline Matrix4x4 Matrix4x4::MakeRotationZ(float rotate) {
	float s = std::sin(rotate);
	float c = std::cos(rotate);
	return {
		c,		s,		0.0f,	0.0f,
		-s,		c,		0.0f,	0.0f,
		0.0f,	0.0f,	1.0f,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f };
}

inline Matrix4x4 Matrix4x4::MakeRotationXYZ(const Vector3& rotate) {
	Vector3 s = { std::sin(rotate.x), std::sin(rotate.y), std::sin(rotate.z) };
	Vector3 c = { std::cos(rotate.x), std::cos(rotate.y), std::cos(rotate.z) };
	return {
		c.y * c.z,						c.y * s.z,						-s.y,		0.0f,
		s.x * s.y * c.z - c.x * s.z,	s.x * s.y * s.z + c.x * c.z,	s.x * c.y,	0.0f,
		c.x * s.y * c.z + s.x * s.z,	c.x * s.y * s.z - s.x * c.z,	c.x * c.y,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f };
}

inline Matrix4x4 Matrix4x4::MakeRotationFromQuaternion(const Quaternion& q) {
	float w2 = q.w * q.w, x2 = q.x * q.x, y2 = q.y * q.y, z2 = q.z * q.z;
	float wx = q.w * q.x, wy = q.w * q.y, wz = q.w * q.z;
	float xy = q.x * q.y, xz = q.x * q.z, yz = q.y * q.z;

	return {
		w2 + x2 - y2 - z2,	2.0f * (wz + xy),	2.0f * (xz - wy),	0.0f,
		2.0f * (xy - wz),	w2 - x2 + y2 - z2,	2.0f * (yz + wx),	0.0f,
		2.0f * (wy + xz),	2.0f * (-wx + yz),	w2 - x2 - y2 + z2,	0.0f,
		0.0f,				0.0f,				0.0f,				1.0f };
}

inline Matrix4x4 Matrix4x4::MakeTranslation(const Vector3& translate) {
	return {
	1.0f,		0.0f,		0.0f,		0.0f,
	0.0f,		1.0f,		0.0f,		0.0f,
	0.0f,		0.0f,		1.0f,		0.0f,
	translate.x,	translate.y,	translate.z,	1.0f };
}

inline Matrix4x4 Matrix4x4::MakeAffine(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Vector3 s = { std::sin(rotate.x), std::sin(rotate.y), std::sin(rotate.z) };
	Vector3 c = { std::cos(rotate.x), std::cos(rotate.y), std::cos(rotate.z) };
	return {
			scale.x * (c.y * c.z),
			scale.x * (c.y * s.z),
			scale.x * (-s.y),
			0.0f,

			scale.y * (s.x * s.y * c.z - c.x * s.z),
			scale.y * (s.x * s.y * s.z + c.x * c.z),
			scale.y * (s.x * c.y),
			0.0f,

			scale.z * (c.x * s.y * c.z + s.x * s.z),
			scale.z * (c.x * s.y * s.z - s.x * c.z),
			scale.z * (c.x * c.y),
			0.0f,

			translate.x,
			translate.y,
			translate.z,
			1.0f
	};
}

inline Matrix4x4 Matrix4x4::MakeAffine(const Vector3& scale, const Quaternion& rotate, const Vector3& translate) {
	float w2 = rotate.w * rotate.w, x2 = rotate.x * rotate.x, y2 = rotate.y * rotate.y, z2 = rotate.z * rotate.z;
	float wx = rotate.w * rotate.x, wy = rotate.w * rotate.y, wz = rotate.w * rotate.z;
	float xy = rotate.x * rotate.y, xz = rotate.x * rotate.z, yz = rotate.y * rotate.z;
	return {
			scale.x * (w2 + x2 - y2 - z2),
			scale.x * (2.0f * (wz + xy)),
			scale.x * (2.0f * (xz - wy)),
			0.0f,

			scale.y * (2.0f * (xy - wz)),
			scale.y * (w2 - x2 + y2 - z2),
			scale.y * (2.0f * (yz + wx)),
			0.0f,

			scale.z * (2.0f * (wy + xz)),
			scale.z * (2.0f * (-wx + yz)),
			scale.z * (w2 - x2 - y2 + z2),
			0.0f,

			translate.x,
			translate.y,
			translate.z,
			1.0f
	};
}

inline Matrix4x4 Matrix4x4::MakeLookRotation(const Vector3& direction, const Vector3& up) {
	Vector3 z = direction.Normalized();
	Vector3 x = Vector3::Cross(up, z).Normalized();
	Vector3 y = Vector3::Cross(z, x);
	return {
		x.x, x.y, x.z, 0.0f,
		y.x, y.y, y.z, 0.0f,
		z.x, z.y, z.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
}

inline Matrix4x4 Matrix4x4::MakePerspectiveProjection(float fovY, float aspect, float nearZ, float farZ) {
	float s = 1.0f / std::tan(fovY * 0.5f);
	float a = farZ / (farZ - nearZ);
	return {
		s / aspect, 0.0f, 0.0f, 0.0f,
		0.0f, s, 0.0f, 0.0f,
		0.0f, 0.0f, a, 1.0f,
		0.0f, 0.0f, a * -nearZ, 0.0f };
}

inline Matrix4x4 Matrix4x4::MakeOrthographicProjection(float width, float height, float nearZ, float farZ) {
	float zRange = farZ - nearZ;
	return {
		2.0f / width, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f / height, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f / zRange, 0.0f,
		0.0f, 0.0f, nearZ / -zRange, 1.0f };
}

inline Matrix4x4 Matrix4x4::MakeViewport(float left, float top, float width, float height, float nearZ, float farZ) {
	float halfW = width / 2.0f;
	float halfh = height / 2.0f;
	return {
		halfW, 0.0f, 0.0f, 0.0f,
		0.0f, -halfh, 0.0f, 0.0f,
		0.0f, 0.0f, farZ - nearZ, 0.0f,
		left + halfW, top + halfh, nearZ, 1.0f };
}
