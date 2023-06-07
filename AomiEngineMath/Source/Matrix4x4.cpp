#include "Matrix4x4.hpp"

const Matrix4x4 Matrix4x4::identity{
	1.0f,0.0f,0.0f,0.0f,
	0.0f,1.0f,0.0f,0.0f,
	0.0f,0.0f,1.0f,0.0f,
	0.0f,0.0f,0.0f,1.0f };

float Matrix4x4::GetDeterminant() {
	float result = 0.0f;

	result += m[0][0] * m[1][1] * m[2][2] * m[3][3]; // +11,22,33,44
	result += m[0][0] * m[1][2] * m[2][3] * m[3][1]; // +11,23,34,42
	result += m[0][0] * m[1][3] * m[2][1] * m[3][2]; // +11,24,32,43

	result -= m[0][0] * m[1][3] * m[2][2] * m[3][1]; // -11,24,33,42
	result -= m[0][0] * m[1][2] * m[2][1] * m[3][3]; // -11,23,32,44
	result -= m[0][0] * m[1][1] * m[2][3] * m[3][2]; // -11,22,34,43

	result -= m[0][1] * m[1][0] * m[2][2] * m[3][3]; // -12,21,33,44
	result -= m[0][2] * m[1][0] * m[2][3] * m[3][1]; // -13,21,34,42
	result -= m[0][3] * m[1][0] * m[2][1] * m[3][2]; // -14,21,32,43

	result += m[0][3] * m[1][0] * m[2][2] * m[3][1]; // +14,21,33,42
	result += m[0][2] * m[1][0] * m[2][1] * m[3][3]; // +13,21,32,44
	result += m[0][1] * m[1][0] * m[2][3] * m[3][2]; // +12,21,34,43

	result += m[0][1] * m[1][2] * m[2][0] * m[3][3]; // +12,23,31,44
	result += m[0][2] * m[1][3] * m[2][0] * m[3][1]; // +13,24,31,42
	result += m[0][3] * m[1][1] * m[2][0] * m[3][2]; // +14,22,31,43

	result -= m[0][3] * m[1][2] * m[2][0] * m[3][1]; // -14,23,31,42
	result -= m[0][2] * m[1][1] * m[2][0] * m[3][3]; // -13,22,31,44
	result -= m[0][1] * m[1][3] * m[2][0] * m[3][2]; // -12,24,31,43

	result -= m[0][1] * m[1][2] * m[2][3] * m[3][0]; // -12,23,34,41
	result -= m[0][2] * m[1][3] * m[2][1] * m[3][0]; // -13,24,32,41
	result -= m[0][3] * m[1][1] * m[2][2] * m[3][0]; // -14,22,33,41

	result += m[0][3] * m[1][2] * m[2][1] * m[3][0]; // +14,23,32,41
	result += m[0][2] * m[1][1] * m[2][3] * m[3][0]; // +13,22,34,41
	result += m[0][1] * m[1][3] * m[2][2] * m[3][0]; // +12,24,33,41

	return result;
}
Matrix4x4 Matrix4x4::GetAdjugate() {
	Matrix4x4 result;
	// 1行目
	result.m[0][0] = 0.0f;							// 11
	result.m[0][0] += m[1][1] * m[2][2] * m[3][3];	// +22,33,44
	result.m[0][0] += m[1][2] * m[2][3] * m[3][1];	// +23,34,42
	result.m[0][0] += m[1][3] * m[2][1] * m[3][2];	// +24,32,43
	result.m[0][0] -= m[1][3] * m[2][2] * m[3][1];	// -24,33,42
	result.m[0][0] -= m[1][2] * m[2][1] * m[3][3];	// -23,32,44
	result.m[0][0] -= m[1][1] * m[2][3] * m[3][2];	// -22,34,43

	result.m[0][1] = 0.0f;							// 12
	result.m[0][1] -= m[0][1] * m[2][2] * m[3][3];	// -12,33,44
	result.m[0][1] -= m[0][2] * m[2][3] * m[3][1];	// -13,34,42
	result.m[0][1] -= m[0][3] * m[2][1] * m[3][2];	// -14,32,43
	result.m[0][1] += m[0][3] * m[2][2] * m[3][1];	// +14,33,42
	result.m[0][1] += m[0][2] * m[2][1] * m[3][3];	// +13,32,44
	result.m[0][1] += m[0][1] * m[2][3] * m[3][2];	// +12,34,43

	result.m[0][2] = 0.0f;							// 13
	result.m[0][2] += m[0][1] * m[1][2] * m[3][3];	// +12,23,44
	result.m[0][2] += m[0][2] * m[1][3] * m[3][1];	// +13,24,42
	result.m[0][2] += m[0][3] * m[1][1] * m[3][2];	// +14,22,43
	result.m[0][2] -= m[0][3] * m[1][2] * m[3][1];	// -14,23,42
	result.m[0][2] -= m[0][2] * m[1][1] * m[3][3];	// -13,22,44
	result.m[0][2] -= m[0][1] * m[1][3] * m[3][2];	// -12,24,43

	result.m[0][3] = 0.0f;							// 14
	result.m[0][3] -= m[0][1] * m[1][2] * m[2][3];	// -12,23,34
	result.m[0][3] -= m[0][2] * m[1][3] * m[2][1];	// -13,24,32
	result.m[0][3] -= m[0][3] * m[1][1] * m[2][2];	// -14,22,33
	result.m[0][3] += m[0][3] * m[1][2] * m[2][1];	// +14,23,32
	result.m[0][3] += m[0][2] * m[1][1] * m[2][3];	// +13,22,34
	result.m[0][3] += m[0][1] * m[1][3] * m[2][2];	// +12,24,33


	// 2行目
	result.m[1][0] = 0.0f;							// 21
	result.m[1][0] -= m[1][0] * m[2][2] * m[3][3];	// -21,33,44
	result.m[1][0] -= m[1][2] * m[2][3] * m[3][0];	// -23,34,41
	result.m[1][0] -= m[1][3] * m[2][0] * m[3][2];	// -24,31,43
	result.m[1][0] += m[1][3] * m[2][2] * m[3][0];	// +24,33,41
	result.m[1][0] += m[1][2] * m[2][0] * m[3][3];	// +23,31,44
	result.m[1][0] += m[1][0] * m[2][3] * m[3][2];	// +21,34,43

	result.m[1][1] = 0.0f;							// 22
	result.m[1][1] += m[0][0] * m[2][2] * m[3][3];	// +11,33,44
	result.m[1][1] += m[0][2] * m[2][3] * m[3][0];	// +13,34,41
	result.m[1][1] += m[0][3] * m[2][0] * m[3][2];	// +14,31,43
	result.m[1][1] -= m[0][3] * m[2][2] * m[3][0];	// -14,33,41
	result.m[1][1] -= m[0][2] * m[2][0] * m[3][3];	// -13,31,44
	result.m[1][1] -= m[0][0] * m[2][3] * m[3][2];	// -11,34,43

	result.m[1][2] = 0.0f;							// 23
	result.m[1][2] -= m[0][0] * m[1][2] * m[3][3];	// -11,23,44
	result.m[1][2] -= m[0][2] * m[1][3] * m[3][0];	// -13,24,41
	result.m[1][2] -= m[0][3] * m[1][0] * m[3][2];	// -14,21,43
	result.m[1][2] += m[0][3] * m[1][2] * m[3][0];	// +14,23,41
	result.m[1][2] += m[0][2] * m[1][0] * m[3][3];	// +13,21,44
	result.m[1][2] += m[0][0] * m[1][3] * m[3][2];	// +11,24,43

	result.m[1][3] = 0.0f;							// 24
	result.m[1][3] += m[0][0] * m[1][2] * m[2][3];	// +11,23,34
	result.m[1][3] += m[0][2] * m[1][3] * m[2][0];	// +13,24,31
	result.m[1][3] += m[0][3] * m[1][0] * m[2][2];	// +14,21,33
	result.m[1][3] -= m[0][3] * m[1][2] * m[2][0];	// -14,23,31
	result.m[1][3] -= m[0][2] * m[1][0] * m[2][3];	// -13,21,34
	result.m[1][3] -= m[0][0] * m[1][3] * m[2][2];	// -11,24,33


	// 3行目
	result.m[2][0] = 0.0f;							// 31
	result.m[2][0] += m[1][0] * m[2][1] * m[3][3];	// +21,32,44
	result.m[2][0] += m[1][1] * m[2][3] * m[3][0];	// +22,34,41
	result.m[2][0] += m[1][3] * m[2][0] * m[3][1];	// +24,31,42
	result.m[2][0] -= m[1][3] * m[2][1] * m[3][0];	// -24,32,41
	result.m[2][0] -= m[1][1] * m[2][0] * m[3][3];	// -22,31,44
	result.m[2][0] -= m[1][0] * m[2][3] * m[3][1];	// -21,34,42

	result.m[2][1] = 0.0f;							// 32
	result.m[2][1] -= m[0][0] * m[2][1] * m[3][3];	// -11,32,44
	result.m[2][1] -= m[0][1] * m[2][3] * m[3][0];	// -12,34,41
	result.m[2][1] -= m[0][3] * m[2][0] * m[3][1];	// -14,31,42
	result.m[2][1] += m[0][3] * m[2][1] * m[3][0];	// +14,32,41
	result.m[2][1] += m[0][1] * m[2][0] * m[3][3];	// +12,31,44
	result.m[2][1] += m[0][0] * m[2][3] * m[3][1];	// +11,34,42

	result.m[2][2] = 0.0f;							// 33
	result.m[2][2] += m[0][0] * m[1][1] * m[3][3];	// +11,22,44
	result.m[2][2] += m[0][1] * m[1][3] * m[3][0];	// +12,24,41
	result.m[2][2] += m[0][3] * m[1][0] * m[3][1];	// +14,21,42
	result.m[2][2] -= m[0][3] * m[1][1] * m[3][0];	// -14,22,41
	result.m[2][2] -= m[0][1] * m[1][0] * m[3][3];	// -12,21,44
	result.m[2][2] -= m[0][0] * m[1][3] * m[3][1];	// -11,24,42

	result.m[2][3] = 0.0f;							// 34
	result.m[2][3] -= m[0][0] * m[1][1] * m[2][3];	// -11,22,34
	result.m[2][3] -= m[0][1] * m[1][3] * m[2][0];	// -12,24,31
	result.m[2][3] -= m[0][3] * m[1][0] * m[2][1];	// -14,21,32
	result.m[2][3] += m[0][3] * m[1][1] * m[2][0];	// +14,22,31
	result.m[2][3] += m[0][1] * m[1][0] * m[2][3];	// +12,21,34
	result.m[2][3] += m[0][0] * m[1][3] * m[2][1];	// +11,24,32


	// 4行目
	result.m[3][0] = 0.0f;							// 41
	result.m[3][0] -= m[1][0] * m[2][1] * m[3][2];	// -21,32,43
	result.m[3][0] -= m[1][1] * m[2][2] * m[3][0];	// -22,33,41
	result.m[3][0] -= m[1][2] * m[2][0] * m[3][1];	// -23,31,42
	result.m[3][0] += m[1][2] * m[2][1] * m[3][0];	// +23,32,41
	result.m[3][0] += m[1][1] * m[2][0] * m[3][2];	// +22,31,43
	result.m[3][0] += m[1][0] * m[2][2] * m[3][1];	// +21,33,42

	result.m[3][1] = 0.0f;							// 42
	result.m[3][1] += m[0][0] * m[2][1] * m[3][2];	// +11,32,43
	result.m[3][1] += m[0][1] * m[2][2] * m[3][0];	// +12,33,41
	result.m[3][1] += m[0][2] * m[2][0] * m[3][1];	// +13,31,42
	result.m[3][1] -= m[0][2] * m[2][1] * m[3][0];	// -13,32,41
	result.m[3][1] -= m[0][1] * m[2][0] * m[3][2];	// -12,31,43
	result.m[3][1] -= m[0][0] * m[2][2] * m[3][1];	// -11,33,42

	result.m[3][2] = 0.0f;							// 43
	result.m[3][2] -= m[0][0] * m[1][1] * m[3][2];	// -11,22,43
	result.m[3][2] -= m[0][1] * m[1][2] * m[3][0];	// -12,23,41
	result.m[3][2] -= m[0][2] * m[1][0] * m[3][1];	// -13,21,42
	result.m[3][2] += m[0][2] * m[1][1] * m[3][0];	// +13,22,41
	result.m[3][2] += m[0][1] * m[1][0] * m[3][2];	// +12,21,43
	result.m[3][2] += m[0][0] * m[1][2] * m[3][1];	// +11,23,42

	result.m[3][3] = 0.0f;							// 44
	result.m[3][3] += m[0][0] * m[1][1] * m[2][2];	// +11,22,33
	result.m[3][3] += m[0][1] * m[1][2] * m[2][0];	// +12,23,31
	result.m[3][3] += m[0][2] * m[1][0] * m[2][1];	// +13,21,32
	result.m[3][3] -= m[0][2] * m[1][1] * m[2][0];	// -13,22,31
	result.m[3][3] -= m[0][1] * m[1][0] * m[2][2];	// -12,21,33
	result.m[3][3] -= m[0][0] * m[1][2] * m[2][1];	// -11,23,32

	return result;
}