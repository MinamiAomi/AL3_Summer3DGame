#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Matrix4x4.h"

namespace Math {

	// 円周率
	constexpr float Pi = 3.141592653589793f;
	// 円周率の2倍
	constexpr float TwoPi = Pi * 2.0f;
	// 円周率の半分
	constexpr float HalfPi = Pi * 0.5f;
	// 弧度法に変換
	constexpr float ToRadian = Pi / 180.0f;
	// 度数法に変換
	constexpr float ToDegree = 180.0f / Pi;

	inline float Lerp(float t, float start, float end);
}

#include "MathUtils_inline.h"
