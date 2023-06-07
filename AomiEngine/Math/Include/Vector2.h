#pragma once
class Vector3;

class Vector2 {
public:
	float x, y;

	static const Vector2 zero;
	static const Vector2 unitX;
	static const Vector2 unitY;
	static const Vector2 one;
	static const Vector2 right;
	static const Vector2 left;
	static const Vector2 up;
	static const Vector2 down;
	
	inline Vector2();
	inline Vector2(float x, float y);
	inline explicit Vector2(float xy);

	inline float& operator[](size_t i);
	inline const float& operator[](size_t i) const;

	inline operator Vector3() const;

	friend inline Vector2 operator+(const Vector2& v);
	friend inline Vector2 operator-(const Vector2& v);
	friend inline Vector2 operator+(const Vector2& v1, const Vector2& v2);
	friend inline Vector2 operator-(const Vector2& v1, const Vector2& v2);
	friend inline Vector2 operator*(const Vector2& v, float s);
	friend inline Vector2 operator*(float s, const Vector2& v);
	friend inline Vector2& operator+=(Vector2& v1, const Vector2& v2);
	friend inline Vector2& operator-=(Vector2& v1, const Vector2& v2);
	friend inline Vector2& operator*=(Vector2& v, float s);
	friend inline bool operator==(const Vector2& v1, const Vector2& v2);
	friend inline bool operator!=(const Vector2& v1, const Vector2& v2);

	/// <summary>
	/// 長さの二乗
	/// </summary>
	__declspec(property(get = LengthSquare)) float lengthSquare;
	inline float LengthSquare() const;
	/// <summary>
	/// 長さ
	/// </summary>
	__declspec(property(get = Length)) float length;
	inline float Length() const;
	/// <summary>
	/// 正規化
	/// </summary>
	__declspec(property(get = Normalized)) Vector2 normalized;
	inline Vector2 Normalized() const;

	/// <summary>
	/// 角度差
	/// </summary>
	/// <param name="from">角度差の元となるベクトル</param>
	/// <param name="to">角度差が測定されるベクトル</param>
	/// <returns></returns>
	static inline float Angle(const Vector2& from, const Vector2& to);
	/// <summary>
	/// 符号付き角度差
	/// </summary>
	/// <param name="from"></param>
	/// <param name="to"></param>
	/// <returns></returns>
	static inline float SignedAngle(const Vector2& from, const Vector2& to);
	/// <summary>
	/// 二つのベクトルの距離
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	static inline float Distance(const Vector2& v1, const Vector2& v2);
	/// <summary>
	/// 内積
	/// </summary>
	/// <param name="lhs"></param>
	/// <param name="rhs"></param>
	/// <returns></returns>
	static inline float Dot(const Vector2& lhs, const Vector2& rhs);
	/// <summary>
	/// 外積
	/// </summary>
	/// <param name="lhs"></param>
	/// <param name="rhs"></param>
	/// <returns></returns>
	static inline float Cross(const Vector2& lhs, const Vector2& rhs);
	/// <summary>
	/// 垂直ベクトル
	/// </summary>
	/// <param name="direction"></param>
	/// <returns></returns>
	static inline Vector2 Perpendicular(const Vector2& direction);
	/// <summary>
	/// それぞれの要素を掛ける
	/// </summary>
	/// <param name="lhs"></param>
	/// <param name="rhs"></param>
	/// <returns></returns>
	static inline Vector2 Scale(const Vector2& lhs, const Vector2& rhs);
	/// <summary>
	/// 正射影ベクトル
	/// </summary>
	/// <param name="base"></param>
	/// <param name="direction"></param>
	/// <returns></returns>
	static inline Vector2 Project(const Vector2& base, const Vector2& direction);
	/// <summary>
	/// 反射ベクトル
	/// </summary>
	/// <param name="direction"></param>
	/// <param name="normal"></param>
	/// <returns></returns>
	static inline Vector2 Reflecte(const Vector2& direction, const Vector2& normal);
	/// <summary>
	/// それぞれの要素の最小
	/// </summary>
	/// <param name="lhs"></param>
	/// <param name="rhs"></param>
	/// <returns></returns>
	static inline Vector2 Min(const Vector2& lhs, const Vector2& rhs);
	/// <summary>
	/// それぞれの要素の最大
	/// </summary>
	/// <param name="lhs"></param>
	/// <param name="rhs"></param>
	/// <returns></returns>
	static inline Vector2 Max(const Vector2& lhs, const Vector2& rhs);
	/// <summary>
	/// 線形補間
	/// </summary>
	/// <param name="t"></param>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <returns></returns>
	static inline Vector2 Lerp(float t, const Vector2& start, const Vector2& end);
	/// <summary>
	/// 球面線形補間
	/// </summary>
	/// <param name="t"></param>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <returns></returns>
	static Vector2 Slerp(float t, const Vector2& start, const Vector2& end);
	/// <summary>
	/// Catmull_Romスプライン曲線
	/// </summary>
	/// <param name="t"></param>
	/// <param name="p0"></param>
	/// <param name="p1"></param>
	/// <param name="p2"></param>
	/// <param name="p3"></param>
	/// <returns></returns>
	Vector2 CatmullRomSpline(float t, const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3);
	/// <summary>
	/// 二次ベジェ曲線
	/// </summary>
	/// <param name="t"></param>
	/// <param name="p0"></param>
	/// <param name="p1"></param>
	/// <param name="p2"></param>
	/// <returns></returns>
	Vector2 QuadraticBezierCurve(float t, const Vector2& p0, const Vector2& p1, const Vector2& p2);
	/// <summary>
	/// 三次ベジェ曲線
	/// </summary>
	/// <param name="t"></param>
	/// <param name="p0"></param>
	/// <param name="p1"></param>
	/// <param name="p2"></param>
	/// <param name="p3"></param>
	/// <returns></returns>
	Vector2 CubicBezierCurve(float t, const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3);
};

#include "Vector2_inline.h"