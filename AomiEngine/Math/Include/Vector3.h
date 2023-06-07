#pragma once
class Vector2;

class Vector3 {
public:
	float x, y, z;

	static const Vector3 zero;
	static const Vector3 unitX;
	static const Vector3 unitY;
	static const Vector3 unitZ;
	static const Vector3 one;
	static const Vector3 right;
	static const Vector3 left;
	static const Vector3 up;
	static const Vector3 down;
	static const Vector3 forward;
	static const Vector3 back;

	inline Vector3();
	inline Vector3(float x, float y, float z);
	inline explicit Vector3(float xyz);
	inline explicit Vector3(const Vector2& xy, float z);

	inline float& operator[](size_t i);
	inline const float& operator[](size_t i) const;

	friend inline Vector3 operator+(const Vector3& v);
	friend inline Vector3 operator-(const Vector3& v);
	friend inline Vector3 operator+(const Vector3& v1, const Vector3& v2);
	friend inline Vector3 operator-(const Vector3& v1, const Vector3& v2);
	friend inline Vector3 operator*(const Vector3& v, float s);
	friend inline Vector3 operator*(float s, const Vector3& v);
	friend inline Vector3& operator+=(Vector3& v1, const Vector3& v2);
	friend inline Vector3& operator-=(Vector3& v1, const Vector3& v2);
	friend inline Vector3& operator*=(Vector3& v, float s);
	friend inline bool operator==(const Vector3& v1, const Vector3& v2);
	friend inline bool operator!=(const Vector3& v1, const Vector3& v2);

	__declspec(property(get = GetXY, put = SetXY)) Vector2 xy;
	inline Vector3& SetXY(const Vector2& xy);
	inline Vector2 GetXY() const;
	__declspec(property(get = GetXZ, put = SetXZ)) Vector2 xz;
	inline Vector3& SetXZ(const Vector2& xz);
	inline Vector2 GetXZ() const;
	__declspec(property(get = GetYZ, put = SetYZ)) Vector2 yz;
	inline Vector3& SetYZ(const Vector2& yz);
	inline Vector2 GetYZ() const;

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
	__declspec(property(get = Normalized)) Vector3 normalized;
	inline Vector3 Normalized() const;

	/// <summary>
	/// 角度差
	/// </summary>
	/// <param name="from">角度差の元となるベクトル</param>
	/// <param name="to">角度差が測定されるベクトル</param>
	/// <returns></returns>
	static inline float Angle(const Vector3& from, const Vector3& to);
	/// <summary>
	/// 符号付き角度差
	/// </summary>
	/// <param name="from"></param>
	/// <param name="to"></param>
	/// <param name="axis"></param>
	/// <returns></returns>
	static inline float SignedAngle(const Vector3& from, const Vector3& to, const Vector3& axis);
	/// <summary>
	/// 二つのベクトルの距離
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	static inline float Distance(const Vector3& v1, const Vector3& v2);
	/// <summary>
	/// 内積
	/// </summary>
	/// <param name="lhs"></param>
	/// <param name="rhs"></param>
	/// <returns></returns>
	static inline float Dot(const Vector3& lhs, const Vector3& rhs);
	/// <summary>
	/// 外積
	/// </summary>
	/// <param name="lhs"></param>
	/// <param name="rhs"></param>
	/// <returns></returns>
	static inline Vector3 Cross(const Vector3& lhs, const Vector3& rhs);
	/// <summary>
	/// それぞれの要素を掛ける
	/// </summary>
	/// <param name="lhs"></param>
	/// <param name="rhs"></param>
	/// <returns></returns>
	static inline Vector3 Scale(const Vector3& lhs, const Vector3& rhs);
	/// <summary>
	/// 正射影ベクトル
	/// </summary>
	/// <param name="base"></param>
	/// <param name="direction"></param>
	/// <returns></returns>
	static inline Vector3 Project(const Vector3& base, const Vector3& direction);
	/// <summary>
	/// 平面に射影したベクトル
	/// </summary>
	/// <param name="base"></param>
	/// <param name="planeNormal">平面の正規化法線</param>
	/// <returns></returns>
	static inline Vector3 ProjectOnPlane(const Vector3& base, const Vector3& planeNormal);
	/// <summary>
	/// 反射ベクトル
	/// </summary>
	/// <param name="direction"></param>
	/// <param name="normal">正規化法線</param>
	/// <returns></returns>
	static inline Vector3 Reflecte(const Vector3& direction, const Vector3& normal);
	/// <summary>
	/// それぞれの要素の最小
	/// </summary>
	/// <param name="lhs"></param>
	/// <param name="rhs"></param>
	/// <returns></returns>
	static inline Vector3 Min(const Vector3& lhs, const Vector3& rhs);
	/// <summary>
	/// それぞれの要素の最大
	/// </summary>
	/// <param name="lhs"></param>
	/// <param name="rhs"></param>
	/// <returns></returns>
	static inline Vector3 Max(const Vector3& lhs, const Vector3& rhs);
	/// <summary>
	/// 線形補間
	/// </summary>
	/// <param name="t"></param>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <returns></returns>
	static inline Vector3 Lerp(float t, const Vector3& start, const Vector3& end);
	/// <summary>
	/// 球面線形補間
	/// </summary>
	/// <param name="t"></param>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <returns></returns>
	static Vector3 Slerp(float t, const Vector3& start, const Vector3& end);
	/// <summary>
	/// Catmull_Romスプライン曲線
	/// </summary>
	/// <param name="t"></param>
	/// <param name="p0"></param>
	/// <param name="p1"></param>
	/// <param name="p2"></param>
	/// <param name="p3"></param>
	/// <returns></returns>
	Vector3 CatmullRomSpline(float t, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3);
	/// <summary>
	/// 二次ベジェ曲線
	/// </summary>
	/// <param name="t"></param>
	/// <param name="p0"></param>
	/// <param name="p1"></param>
	/// <param name="p2"></param>
	/// <returns></returns>
	Vector3 QuadraticBezierCurve(float t, const Vector3& p0, const Vector3& p1, const Vector3& p2);
	/// <summary>
	/// 三次ベジェ曲線
	/// </summary>
	/// <param name="t"></param>
	/// <param name="p0"></param>
	/// <param name="p1"></param>
	/// <param name="p2"></param>
	/// <param name="p3"></param>
	/// <returns></returns>
	Vector3 CubicBezierCurve(float t, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3);
};

#include "Vector3_inline.h"