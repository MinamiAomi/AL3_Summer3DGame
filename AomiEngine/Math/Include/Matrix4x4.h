#pragma once
#include "Vector3.h"
class Vector4;
class Quaternion;

class Matrix4x4 {
public:
	float m[4][4];

	static const Matrix4x4 identity;

	inline Matrix4x4();
	inline Matrix4x4(
		float _00, float _01, float _02, float _03,
		float _10, float _11, float _12, float _13,
		float _20, float _21, float _22, float _23,
		float _30, float _31, float _32, float _33);

	friend inline Matrix4x4 operator*(const Matrix4x4& lhs, const Matrix4x4& rhs);
	friend inline Matrix4x4& operator*=(Matrix4x4& lhs, const Matrix4x4& rhs);
	friend inline Vector3 operator*(const Vector3& lhs, const Matrix4x4& rhs);
	friend inline Vector4 operator*(const Vector4& lhs, const Matrix4x4& rhs);
	friend inline Matrix4x4 operator*(float lhs, const Matrix4x4& rhs);
	friend inline Matrix4x4 operator*(const Matrix4x4& lhs, float rhs);

	/// <summary>
	/// ベクトルに回転を適用
	/// </summary>
	/// <param name="vector"></param>
	/// <returns></returns>
	inline Vector3 ApplyRotation(const Vector3& vector) const;
	/// <summary>
	/// ベクトルに変換を適用（Wで割る）
	/// </summary>
	/// <param name="vector"></param>
	/// <returns></returns>
	inline Vector3 ApplyTransformWDivide(const Vector3& vector) const;

	/// <summary>
	/// 行
	/// </summary>
	__declspec(property(get = GetRow, put = SetRow)) Vector4 row[];
	inline Matrix4x4& SetRow(size_t i, const Vector4& v);
	inline Vector4 GetRow(size_t i) const;
	/// <summary>
	/// 列
	/// </summary>
	__declspec(property(get = GetColumn, put = SetColumn)) Vector4 column[];
	inline Matrix4x4& SetColumn(size_t i, const Vector4& v);
	inline Vector4 GetColumn(size_t i) const;
	/// <summary>
	/// X軸の向き（読み取り専用）
	/// </summary>
	__declspec(property(get = GetXAxis)) Vector3 xAxis;
	inline Vector3 GetXAxis() const;
	/// <summary>
	/// Y軸の向き（読み取り専用）
	/// </summary>
	__declspec(property(get = GetXAxis)) Vector3 yAxis;
	inline Vector3 GetYAxis() const;
	/// <summary>
	/// Z軸の向き（読み取り専用）
	/// </summary>
	__declspec(property(get = GetXAxis)) Vector3 zAxis;
	inline Vector3 GetZAxis() const;
	/// <summary>
	/// 平行移動成分（読み取り専用）
	/// </summary>
	__declspec(property(get = GetTranslate)) Vector3 translate;
	inline Vector3 GetTranslate() const;
	/// <summary>
	/// 行列式（読み取り専用）
	/// </summary>
	__declspec(property(get = GetDeterminant)) float determinant;
	float GetDeterminant();
	/// <summary>
	/// 余因子行列（読み取り専用）
	/// </summary>
	__declspec(property(get = GetAdjugate)) Matrix4x4 adjugate;
	Matrix4x4 GetAdjugate();
	/// <summary>
	/// 逆行列（読み取り専用）
	/// </summary>
	__declspec(property(get = GetInverse)) Matrix4x4 inverse;
	inline Matrix4x4 GetInverse();
	/// <summary>
	/// 転置行列（読み取り専用）
	/// </summary>
	__declspec(property(get = GetTranspose)) Matrix4x4 transpose;
	inline Matrix4x4 GetTranspose();

	/// <summary>
	/// 拡大縮小行列
	/// </summary>
	/// <param name="scale"></param>
	/// <returns></returns>
	static inline Matrix4x4 MakeScaling(const Vector3& scale);
	/// <summary>
	/// X軸回りの回転行列
	/// </summary>
	/// <param name="rotate"></param>
	/// <returns></returns>
	static inline Matrix4x4 MakeRotationX(float rotate);
	/// <summary>
	/// Y軸回りの回転行列
	/// </summary>
	/// <param name="rotate"></param>
	/// <returns></returns>
	static inline Matrix4x4 MakeRotationY(float rotate);
	/// <summary>
	/// Z軸回りの回転行列
	/// </summary>
	/// <param name="rotate"></param>
	/// <returns></returns>
	static inline Matrix4x4 MakeRotationZ(float rotate);
	/// <summary>
	/// XYZ順の回転行列
	/// </summary>
	/// <param name="rotate"></param>
	/// <returns></returns>
	static inline Matrix4x4 MakeRotationXYZ(const Vector3& rotate);
	/// <summary>
	/// クォータニオンから回転行列
	/// </summary>
	/// <param name="q"></param>
	/// <returns></returns>
	static inline Matrix4x4 MakeRotationFromQuaternion(const Quaternion& q);
	/// <summary>
	/// 特定の方向に向ける回転行列
	/// </summary>
	/// <param name="direction"></param>
	/// <param name="up"></param>
	/// <returns></returns>
	static inline Matrix4x4 MakeLookRotation(const Vector3& direction, const Vector3& up = Vector3::up);
	/// <summary>
	/// 平行移動行列
	/// </summary>
	/// <param name="translate"></param>
	/// <returns></returns>
	static inline Matrix4x4 MakeTranslation(const Vector3& translate);
	/// <summary>
	/// アフィン変換行列
	/// </summary>
	/// <param name="scale"></param>
	/// <param name="rotate"></param>
	/// <param name="translate"></param>
	/// <returns></returns>
	static inline Matrix4x4 MakeAffine(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
	/// <summary>
	/// アフィン変換行列
	/// </summary>
	/// <param name="scale"></param>
	/// <param name="rotate"></param>
	/// <param name="translate"></param>
	/// <returns></returns>
	static inline Matrix4x4 MakeAffine(const Vector3& scale, const Quaternion& rotate, const Vector3& translate);
	/// <summary>
	/// 透視投影行列
	/// </summary>
	/// <param name="fovY"></param>
	/// <param name="aspect"></param>
	/// <param name="nearZ"></param>
	/// <param name="farZ"></param>
	/// <returns></returns>
	static inline Matrix4x4 MakePerspectiveProjection(float fovY, float aspect, float nearZ, float farZ);
	/// <summary>
	/// 平行投影行列
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="nearZ"></param>
	/// <param name="farZ"></param>
	/// <returns></returns>
	static inline Matrix4x4 MakeOrthographicProjection(float width, float height, float nearZ, float farZ);
	/// <summary>
	/// ビューポート行列
	/// </summary>
	/// <param name="left"></param>
	/// <param name="top"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="nearZ"></param>
	/// <param name="farZ"></param>
	/// <returns></returns>
	static inline Matrix4x4 MakeViewport(float left, float top, float width, float height, float nearZ = 0.0f, float farZ = 1.0f);

};

#include "Matrix4x4_inline.h"