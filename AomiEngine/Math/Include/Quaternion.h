#pragma once
#include "Vector3.h"
class Matrix4x4;

class Quaternion {
public:
	float x, y, z, w;

	static const Quaternion identity;

	inline Quaternion();
	inline explicit Quaternion(float x, float y, float z, float w);

	inline float& operator[](size_t i);
	inline const float& operator[](size_t i) const;

	friend inline Quaternion operator+(const Quaternion& q1, const Quaternion& q2);
	friend inline Quaternion operator*(const Quaternion& q, float s);
	friend inline Quaternion operator*(float s, const Quaternion& q);
	friend inline Vector3 operator*(const Quaternion& lhs, const Vector3& rhs);
	friend inline Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs);
	friend inline Quaternion& operator+=(Quaternion& v1, const Quaternion& v2);
	friend inline Quaternion& operator*=(Quaternion& v, float s);
	friend inline Quaternion& operator*=(Quaternion lhs, const Quaternion& rhs);
	friend inline bool operator==(const Quaternion& v1, const Quaternion& v2);
	friend inline bool operator!=(const Quaternion& v1, const Quaternion& v2);

	/// <summary>
	/// オイラー角（不安）
	/// </summary>
	__declspec(property(get = GetEulerAngle, put = SetEulerAngle)) Vector3 eulerAngle;
	inline Quaternion& SetEulerAngle(const Vector3& eulerAngle);
	inline Vector3 GetEulerAngle() const;
	/// <summary>
	/// 正規化（読み取り専用）
	/// </summary>
	__declspec(property(get = Normalized)) Quaternion normalized;
	inline Quaternion Normalized() const;
	/// <summary>
	/// xyz（読み取り専用）
	/// </summary>
	__declspec(property(get = GetXYZ)) Vector3 xyz;
	inline Vector3 GetXYZ() const;
	/// <summary>
	/// 回転角
	/// </summary>
	__declspec(property(get = GetAngle, put = SetAngle)) float angle;
	inline Quaternion& SetAngle(float angle);
	inline float GetAngle() const;
	/// <summary>
	/// 回転軸
	/// </summary>
	__declspec(property(get = GetAxis, put = SetAxis)) Vector3 axis;
	inline Quaternion& SetAxis(const Vector3& axis);
	inline Vector3 GetAxis() const;
	/// <summary>
	/// 共役（読み取り専用）
	/// </summary>
	__declspec(property(get = GetConjugate)) Quaternion conjugate;
	inline Quaternion GetConjugate();
	/// <summary>
	/// 逆クォータニオン（読み取り専用）
	/// </summary>
	__declspec(property(get = GetInverse)) Quaternion inverse;
	inline Quaternion GetInverse();
	/// <summary>
	/// 内積
	/// </summary>
	/// <param name="lhs"></param>
	/// <param name="rhs"></param>
	/// <returns></returns>
	static inline float Dot(const Quaternion& lhs, const Quaternion& rhs);
	/// <summary>
	/// 線形補間
	/// </summary>
	/// <param name="t"></param>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <returns></returns>
	static inline Quaternion Lerp(float t, const Quaternion& start, const Quaternion& end);
	/// <summary>
	/// 球面線形補間
	/// </summary>
	/// <param name="t"></param>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <returns></returns>
	static Quaternion Slerp(float t, const Quaternion& start, const Quaternion& end);
	/// <summary>
	/// 軸と角度から生成
	/// </summary>
	/// <param name="angle"></param>
	/// <param name="axis"></param>
	/// <returns></returns>
	static inline Quaternion MakeFromAngleAxis(float angle, const Vector3& axis);
	/// <summary>
	/// オイラー角から生成
	/// </summary>
	/// <param name="euler"></param>
	/// <returns></returns>
	static inline Quaternion MakeFromEulerAngle(const Vector3& euler);
	/// <summary>
	/// X軸回り
	/// </summary>
	/// <param name="angle"></param>
	/// <returns></returns>
	static inline Quaternion MakeForXAxis(float angle);
	/// <summary>
	/// Y軸回り
	/// </summary>
	/// <param name="angle"></param>
	/// <returns></returns>
	static inline Quaternion MakeForYAxis(float angle);
	/// <summary>
	/// Z軸回り
	/// </summary>
	/// <param name="angle"></param>
	/// <returns></returns>
	static inline Quaternion MakeForZAxis(float angle);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	static inline Quaternion MakeFromTwoVector(const Vector3& v1, const Vector3& v2);
	static inline Quaternion MakeFromOrthonormal(const Vector3& x, const Vector3& y, const Vector3& z);
	static inline Quaternion MakeLookRotation(const Vector3& direction, const Vector3& up = Vector3::up);
	static inline Quaternion MakeFromMatrix(const Matrix4x4& m);
};

#include "Quaternion_inline.h"