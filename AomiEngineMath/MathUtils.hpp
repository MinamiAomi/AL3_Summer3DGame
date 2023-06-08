#pragma once

#include <cmath>
#include <algorithm>

namespace AomiEngine {

	// 数学系
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

		// 線形補間
		inline constexpr float Lerp(float t, float start, float end) {
			return start + t * (end - start);
		}
	}

#pragma region 前方宣言
	class Vector2;
	class Vector3;
	class Vector4;
	class Quaternion;
	class Matrix4x4;
#pragma endregion

	// 二次元ベクトル
	class Vector2 {
	public:
#pragma region 定数
		static const Vector2 zero;
		static const Vector2 unitX;
		static const Vector2 unitY;
		static const Vector2 one;
		static const Vector2 right;
		static const Vector2 left;
		static const Vector2 up;
		static const Vector2 down;
#pragma endregion
#pragma region コンストラクタ
		inline constexpr Vector2() noexcept : x(0.0f), y(0.0f) {}
		inline constexpr Vector2(float x, float y) noexcept : x(x), y(y) {}
		inline constexpr explicit Vector2(float xy) noexcept : x(xy), y(xy) {}
#pragma endregion
#pragma region 演算子のオーバーロード
		inline constexpr const float& operator[](size_t index) const noexcept {
			return *(&x + index);
		}
		inline constexpr float& operator[](size_t index) noexcept {
			return const_cast<float&>(static_cast<const Vector2&>(*this)[index]);
		}

		// inline constexpr explicit operator Vector3() const;

		friend inline constexpr Vector2 operator+(const Vector2& rhs) noexcept {
			return rhs;
		}
		friend inline constexpr Vector2 operator-(const Vector2& rhs) noexcept {
			return { -rhs.x, -rhs.y };
		}
		friend inline constexpr Vector2 operator+(const Vector2& lhs, const Vector2& rhs) noexcept {
			return { lhs.x + rhs.x, lhs.y + rhs.y };
		}
		friend inline constexpr Vector2 operator-(const Vector2& lhs, const Vector2& rhs) noexcept {
			return { lhs.x - rhs.x, lhs.y - rhs.y };
		}
		friend inline constexpr Vector2 operator*(const Vector2& lhs, float rhs) noexcept {
			return { lhs.x * rhs, lhs.y * rhs };
		}
		friend inline constexpr Vector2 operator*(float lhs, const Vector2& rhs) noexcept {
			return { lhs * rhs.x, lhs * rhs.y };
		}
		friend inline constexpr Vector2 operator/(const Vector2& lhs, float rhs) noexcept {
			return lhs * (1.0f / rhs);
		}
		friend inline constexpr Vector2& operator+=(Vector2& lhs, const Vector2& rhs) noexcept {
			lhs = lhs + rhs;
			return lhs;
		}
		friend inline constexpr Vector2& operator-=(Vector2& lhs, const Vector2& rhs) noexcept {
			lhs = lhs - rhs;
			return lhs;
		}
		friend inline constexpr Vector2& operator*=(Vector2& lhs, float rhs) noexcept {
			lhs = lhs * rhs;
			return lhs;
		}
		friend inline constexpr Vector2& operator/=(Vector2& lhs, float rhs) noexcept {
			lhs = lhs / rhs;
			return lhs;
		}
		friend inline constexpr bool operator==(const Vector2& lhs, const Vector2& rhs) noexcept {
			return lhs.x == rhs.x && lhs.y == rhs.y;
		}
		friend inline constexpr bool operator!=(const Vector2& lhs, const Vector2& rhs) noexcept {
			return !(lhs == rhs);
		}
#pragma endregion
#pragma region メンバ関数
		/// 長さの二乗
		inline constexpr float LengthSquare() const noexcept {
			return x * x + y * y;
		}
		/// 長さ
		inline float Length() const noexcept {
			return std::sqrt(LengthSquare());
		}
		/// 正規化
		inline Vector2 Normalized() const noexcept {
			return *this / Length();
		}
#pragma endregion
#pragma region 静的関数
		// 角度差
		static inline float Angle(const Vector2& from, const Vector2& to) noexcept {
			return std::acos(Dot(from.Normalized(), to.Normalized()));
		}
		// 符号付き角度差
		static inline float SignedAngle(const Vector2& from, const Vector2& to) noexcept {
			return Cross(from, to) >= 0.0f ? Angle(from, to) : -Angle(from, to);
		}
		// 距離
		static inline float Distance(const Vector2& start, const Vector2& end) noexcept {
			return (end - start).Length();
		}
		// 内積
		static inline constexpr float Dot(const Vector2& lhs, const Vector2& rhs) noexcept {
			return lhs.x * rhs.x + lhs.y * rhs.y;
		}
		// 外積
		static inline constexpr float Cross(const Vector2& lhs, const Vector2& rhs) noexcept {
			return lhs.x * rhs.y - lhs.y * rhs.x;
		}
		// 垂直
		static inline constexpr Vector2 Perpendicular(const Vector2& direction) noexcept {
			return { -direction.y, direction.x };
		}
		// それぞれの要素を乗算
		static inline constexpr Vector2 Scale(const Vector2& lhs, const Vector2& rhs) noexcept {
			return { lhs.x * rhs.x, lhs.y * rhs.y };
		}
		// 正射影
		static inline constexpr Vector2 Project(const Vector2& base, const Vector2& direction) noexcept {
			return Dot(base, direction.Normalized()) * direction.Normalized();
		}
		// 反射
		static inline constexpr Vector2 Reflecte(const Vector2& direction, const Vector2& normal) noexcept {
			return Dot(normal.Normalized(), -direction) * 2.0f * normal.Normalized() + direction;
		}
		// それぞれの要素の最小
		static inline constexpr Vector2 Min(const Vector2& lhs, const Vector2& rhs) noexcept {
			return { std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y) };
		}
		// それぞれの要素の最大											  
		static inline constexpr Vector2 Max(const Vector2& lhs, const Vector2& rhs) noexcept {
			return { std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y) };
		}
		// それぞれの要素を収める
		static inline constexpr Vector2 Clamp(const Vector2& value, const Vector2& min, const Vector2& max) noexcept {
			return { std::clamp(value.x, min.x, max.x), std::clamp(value.y, min.y, max.y) };
		}
		// 線形補間
		static inline constexpr Vector2 Lerp(float t, const Vector2& start, const Vector2& end) noexcept {
			return start + t * (end - start);
		}
		// 球面線形補間
		static Vector2 Slerp(float t, const Vector2& start, const Vector2& end) noexcept;
		// Catmull_Romスプライン曲線
		static Vector2 CatmullRomSpline(float t, const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3) noexcept;
		// 二次ベジェ曲線
		static Vector2 QuadraticBezierCurve(float t, const Vector2& p0, const Vector2& p1, const Vector2& p2) noexcept;
		// 三次ベジェ曲線
		static Vector2 CubicBezierCurve(float t, const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3) noexcept;
#pragma endregion
#pragma region メンバ変数
		float x, y;
#pragma endregion
	};

	// 三次元ベクトル
	class Vector3 {
	public:
#pragma region 定数
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
#pragma endregion
#pragma region コンストラクタ
		inline constexpr Vector3() noexcept : x(0.0f), y(0.0f), z(0.0f) {}
		inline constexpr Vector3(float x, float y, float z) noexcept : x(x), y(y), z(z) {}
		inline constexpr Vector3(const Vector2& xy, float z) noexcept : x(xy.x), y(xy.y), z(z) {}
		inline constexpr explicit Vector3(float xyz) noexcept : x(xyz), y(xyz), z(xyz) {}
#pragma endregion
#pragma region 演算子のオーバーロード
		inline constexpr const float& operator[](std::size_t index) const noexcept {
			return *(&x + index);
		}
		inline constexpr float& operator[](std::size_t index) noexcept {
			return const_cast<float&>(static_cast<const Vector3&>(*this)[index]);
		}

		friend inline constexpr Vector3 operator+(const Vector3& rhs) noexcept {
			return rhs;
		}
		friend inline constexpr Vector3 operator-(const Vector3& rhs) noexcept {
			return { -rhs.x, -rhs.y, -rhs.z };
		}
		friend inline constexpr Vector3 operator+(const Vector3& lhs, const Vector3& rhs) noexcept {
			return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
		}
		friend inline constexpr Vector3 operator-(const Vector3& lhs, const Vector3& rhs) noexcept {
			return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
		}
		friend inline constexpr Vector3 operator*(const Vector3& lhs, float rhs) noexcept {
			return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs };
		}
		friend inline constexpr Vector3 operator*(float lhs, const Vector3& rhs) noexcept {
			return { lhs * rhs.x, lhs * rhs.y, lhs * rhs.z };
		}
		friend inline constexpr Vector3 operator/(const Vector3& lhs, float rhs) noexcept {
			return lhs * (1.0f / rhs);
		}
		friend inline constexpr Vector3& operator+=(Vector3& lhs, const Vector3& rhs) noexcept {
			lhs = lhs + rhs;
			return lhs;
		}
		friend inline constexpr Vector3& operator-=(Vector3& lhs, const Vector3& rhs) noexcept {
			lhs = lhs - rhs;
			return lhs;
		}
		friend inline constexpr Vector3& operator*=(Vector3& lhs, float rhs) noexcept {
			lhs = lhs * rhs;
			return lhs;
		}
		friend inline constexpr Vector3& operator/=(Vector3& lhs, float rhs) noexcept {
			lhs = lhs / rhs;
			return lhs;
		}
		friend inline constexpr bool operator==(const Vector3& lhs, const Vector3& rhs) noexcept {
			return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
		}
		friend inline constexpr bool operator!=(const Vector3& lhs, const Vector3& rhs) noexcept {
			return !(lhs == rhs);
		}
#pragma endregion
#pragma region メンバ関数
		inline constexpr Vector3& xy(const Vector2& xy) noexcept {
			x = xy.x, y = xy.y;
			return *this;
		}
		inline constexpr Vector2 xy() const noexcept {
			return { x, y };
		}
		inline constexpr Vector3& xz(const Vector2& xz) noexcept {
			x = xz.x, z = xz.y;
			return *this;
		}
		inline constexpr Vector2 xz() const noexcept {
			return { x, z };
		}
		inline constexpr Vector3& yz(const Vector2& yz) noexcept {
			y = yz.x, z = yz.y;
			return *this;
		}
		inline constexpr Vector2 yz() const noexcept {
			return { y, z };
		}

		// 長さの二乗
		inline constexpr float LengthSquare() const noexcept {
			return x * x + y * y + z * z;
		}
		// 長さ
		inline float Length() const noexcept {
			return std::sqrt(LengthSquare());
		}
		// 正規化
		inline Vector3 Normalized() const noexcept {
			return *this / Length();
		}
#pragma endregion
#pragma region 静的関数
		// 角度差
		static inline float Angle(const Vector3& from, const Vector3& to) noexcept {
			return std::acos(Dot(from.Normalized(), to.Normalized()));
		}
		// 符号付き角度差
		static inline float SignedAngle(const Vector3& from, const Vector3& to, const Vector3& axis) noexcept {
			return Dot(Cross(from, to), axis) < 0.0f ? Angle(from, to) : -Angle(from, to);
		}
		// 二つのベクトルの距離
		static inline float Distance(const Vector3& start, const Vector3& end) noexcept {
			return (end - start).Length();
		}
		// 内積
		static inline constexpr float Dot(const Vector3& lhs, const Vector3& rhs) noexcept {
			return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
		}
		// 外積
		static inline constexpr Vector3 Cross(const Vector3& lhs, const Vector3& rhs) noexcept {
			return {
					lhs.y * rhs.z - lhs.z * rhs.y,
					lhs.z * rhs.x - lhs.x * rhs.z,
					lhs.x * rhs.y - lhs.y * rhs.x };
		}
		// それぞれの要素を掛ける
		static inline constexpr Vector3 Scale(const Vector3& lhs, const Vector3& rhs) noexcept {
			return { lhs.x * rhs.x , lhs.y + rhs.y, lhs.z * rhs.z };
		}
		// 正射影ベクトル
		static inline constexpr Vector3 Project(const Vector3& base, const Vector3& direction) noexcept {
			return Dot(base, direction.Normalized()) * direction.Normalized();
		}
		// 平面に射影したベクトル
		static inline constexpr Vector3 ProjectOnPlane(const Vector3& base, const Vector3& planeNormal) noexcept {
			return base - planeNormal.Normalized() * Dot(base, planeNormal.Normalized());
		}
		// 反射ベクトル
		static inline constexpr Vector3 Reflecte(const Vector3& direction, const Vector3& normal) noexcept {
			return Dot(normal.Normalized(), -direction) * 2.0f * normal.Normalized() + direction;
		}
		// それぞれの要素の最小
		static inline constexpr Vector3 Min(const Vector3& lhs, const Vector3& rhs) noexcept {
			return { std::min(lhs.x,rhs.x), std::min(lhs.y,rhs.y), std::min(lhs.z,rhs.z) };
		}
		// それぞれの要素の最大
		static inline constexpr Vector3 Max(const Vector3& lhs, const Vector3& rhs) noexcept {
			return { std::max(lhs.x,rhs.x), std::max(lhs.y,rhs.y), std::max(lhs.z,rhs.z) };
		}
		// それぞれの要素を収める
		static inline constexpr Vector3 Clamp(const Vector3& value, const Vector3& min, const Vector3& max) noexcept {
			return { std::clamp(value.x, min.x, max.x), std::clamp(value.y, min.y, max.y), std::clamp(value.z, min.z, max.z) };
		}
		// 線形補間
		static inline constexpr Vector3 Lerp(float t, const Vector3& start, const Vector3& end) noexcept {
			return start + t * (end - start);
		}
		// 球面線形補間
		static Vector3 Slerp(float t, const Vector3& start, const Vector3& end) noexcept;
		// Catmull_Romスプライン曲線
		static Vector3 CatmullRomSpline(float t, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3) noexcept;
		// 二次ベジェ曲線
		static Vector3 QuadraticBezierCurve(float t, const Vector3& p0, const Vector3& p1, const Vector3& p2) noexcept;
		// 三次ベジェ曲線
		static Vector3 CubicBezierCurve(float t, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3) noexcept;
#pragma endregion
#pragma region メンバ変数
		float x, y, z;
#pragma endregion
	};

	// 四次元ベクトル
	class Vector4 {
	public:
#pragma region 定数
		static const Vector4 zero;
		static const Vector4 one;
#pragma endregion
#pragma region コンストラクタ
		inline constexpr Vector4() noexcept : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
		inline constexpr Vector4(float x, float y, float z, float w) noexcept : x(x), y(y), z(z), w(w) {}
		inline constexpr Vector4(const Vector3& xyz, float w) noexcept : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
		inline constexpr explicit Vector4(float xyzw) noexcept : x(xyzw), y(xyzw), z(xyzw), w(xyzw) {}
#pragma endregion
#pragma region 演算子のオーバーロード
		inline constexpr const float& operator[](std::size_t index) const noexcept {
			return *(&x + index);
		}
		inline constexpr float& operator[](std::size_t index) noexcept {
			return const_cast<float&>(static_cast<const Vector4&>(*this)[index]);
		}
		inline constexpr explicit operator Vector3() const noexcept {
			return { x, y, z };
		}

		friend inline constexpr Vector4 operator+(const Vector4& rhs) noexcept {
			return rhs;
		}
		friend inline constexpr Vector4 operator-(const Vector4& rhs) noexcept {
			return { -rhs.x, -rhs.y, -rhs.z, -rhs.w };
		}
		friend inline constexpr Vector4 operator+(const Vector4& lhs, const Vector4& rhs) noexcept {
			return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w };
		}
		friend inline constexpr Vector4 operator-(const Vector4& lhs, const Vector4& rhs) noexcept {
			return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w };
		}
		friend inline constexpr Vector4 operator*(const Vector4& lhs, float rhs) noexcept {
			return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs };
		}
		friend inline constexpr Vector4 operator*(float lhs, const Vector4& rhs) noexcept {
			return { lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w };
		}
		friend inline constexpr Vector4 operator/(const Vector4& lhs, float rhs) noexcept {
			return lhs * (1.0f / rhs);
		}
		friend inline constexpr Vector4& operator+=(Vector4& lhs, const Vector4& rhs) noexcept {
			lhs = lhs + rhs;
			return lhs;
		}
		friend inline constexpr Vector4& operator-=(Vector4& lhs, const Vector4& rhs) noexcept {
			lhs = lhs - rhs;
			return lhs;
		}
		friend inline constexpr Vector4& operator*=(Vector4& lhs, float rhs) noexcept {
			lhs = lhs * rhs;
			return lhs;
		}
		friend inline constexpr Vector4& operator/=(Vector4& lhs, float rhs) noexcept {
			lhs = lhs / rhs;
			return lhs;
		}
		friend inline constexpr bool operator==(const Vector4& lhs, const Vector4& rhs) noexcept {
			return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
		}
		friend inline constexpr bool operator!=(const Vector4& lhs, const Vector4& rhs) noexcept {
			return !(lhs == rhs);
		}
#pragma endregion
#pragma region メンバ関数
		inline constexpr Vector4& xyz(const Vector3& xyz) noexcept {
			x = xyz.x, y = xyz.y, z = xyz.z;
			return *this;
		}
		inline constexpr Vector3 xyz() const noexcept {
			return { x, y, z };
		}

		/// 長さの二乗
		inline constexpr float LengthSquare() const noexcept {
			return x * x + y * y + z * z + w * w;
		}
		/// 長さ
		inline float Length() const noexcept {
			return std::sqrt(LengthSquare());
		}
		/// 正規化
		inline Vector4 Normalized() const noexcept {
			return *this / Length();
		}
#pragma endregion
#pragma region 静的関数
		// 内積
		static inline constexpr float Dot(const Vector4& lhs, const Vector4& rhs) noexcept {
			return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
		}
		// それぞれの要素を掛ける
		static inline constexpr Vector4 Scale(const Vector4& lhs, const Vector4& rhs) noexcept {
			return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.z * rhs.z };
		}
		// 正射影ベクトル
		static inline Vector4 Project(const Vector4& base, const Vector4& direction) noexcept {
			return Dot(base, direction.Normalized()) * direction.Normalized();
		}
		// それぞれの要素の最小
		static inline constexpr Vector4 Min(const Vector4& lhs, const Vector4& rhs) noexcept {
			return { std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z), std::min(lhs.w, rhs.w) };
		}
		// それぞれの要素の最大
		static inline constexpr Vector4 Max(const Vector4& lhs, const Vector4& rhs) noexcept {
			return { std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z), std::max(lhs.w, rhs.w) };
		}
		// それぞれの要素を収める
		static inline constexpr Vector4 Clamp(const Vector4& value, const Vector4& min, const Vector4& max) noexcept {
			return { std::clamp(value.x, min.x, max.x), std::clamp(value.y, min.y, max.y), std::clamp(value.z, min.z, max.z), std::clamp(value.w, min.w, max.w) };
		}
		// 線形補間
		static inline constexpr Vector4 Lerp(float t, const Vector4& start, const Vector4& end) noexcept {
			return start + t * (end - start);
		}
#pragma endregion
#pragma region メンバ変数
		float x, y, z, w;
#pragma endregion 
	};

	// 四元数
	class Quaternion {
	public:
#pragma region 定数
		static const Quaternion identity;
#pragma endregion
#pragma region ファクトリ関数
		// 軸と角度から生成
		static inline Quaternion MakeFromAngleAxis(float angle, const Vector3& axis) noexcept {
			Vector3 v = axis.Normalized() * std::sin(angle * 0.5f);
			return Quaternion{
				v.x,
				v.y,
				v.z,
				std::cos(angle * 0.5f) };
		}
		// オイラー角から生成
		static inline Quaternion MakeFromEulerAngle(const Vector3& euler) noexcept {
			Vector3 s = Vector3(std::sin(euler.x * 0.5f), std::sin(euler.y * 0.5f), std::sin(euler.z * 0.5f));
			Vector3 c = Vector3(std::cos(euler.x * 0.5f), std::cos(euler.y * 0.5f), std::cos(euler.z * 0.5f));
			return Quaternion{
				s.x * c.y * c.z - c.x * s.y * s.z,
				c.x * s.y * c.z + s.x * c.y * s.z,
				c.x * c.y * s.z - s.x * s.y * c.z,
				c.x * c.y * c.z + s.x * s.y * s.z };
		}
		// X軸回り
		static inline Quaternion MakeForXAxis(float angle) noexcept {
			return Quaternion(std::sin(angle / 2.0f), 0.0f, 0.0f, std::cos(angle / 2.0f));
		}
		// Y軸回り
		static inline Quaternion MakeForYAxis(float angle) noexcept {
			return Quaternion(0.0f, std::sin(angle / 2.0f), 0.0f, std::cos(angle / 2.0f));
		}
		// Z軸回り
		static inline Quaternion MakeForZAxis(float angle) noexcept {
			return Quaternion(0.0f, 0.0f, std::sin(angle / 2.0f), std::cos(angle / 2.0f));
		}

		// 二つのベクトルに垂直な軸と差分角から生成
		static inline Quaternion MakeFromTwoVector(const Vector3& from, const Vector3& to) noexcept {
			Vector3 axis = Vector3::Cross(from, to);
			float angle = Vector3::Angle(from, to);
			return MakeFromAngleAxis(angle, axis);
		}
		// 正規直交軸から生成
		static Quaternion MakeFromOrthonormal(const Vector3& x, const Vector3& y, const Vector3& z) noexcept;
		// ルッククォータニオンを生成
		static inline Quaternion MakeLookRotation(const Vector3& direction, const Vector3& up = Vector3::up) noexcept {
			Vector3 z = direction.Normalized();
			Vector3 x = Vector3::Cross(up, z).Normalized();
			Vector3 y = Vector3::Cross(z, x);
			return MakeFromOrthonormal(x, y, z);
		}
#pragma endregion
#pragma region コンストラクタ
		inline constexpr Quaternion() noexcept : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
		inline constexpr explicit Quaternion(float x, float y, float z, float w) noexcept : x(x), y(y), z(z), w(w) {}
#pragma endregion
#pragma region 演算子のオーバーロード
		inline constexpr const float& operator[](size_t index) const noexcept {
			return *(&x + index);
		}
		inline constexpr float& operator[](size_t index) noexcept {
			return const_cast<float&>(static_cast<const Quaternion&>(*this)[index]);
		}

		friend inline constexpr Quaternion operator+(const Quaternion& lhs, const Quaternion& rhs) noexcept {
			return Quaternion{ lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w };
		}
		friend inline constexpr Quaternion operator*(const Quaternion& lhs, float rhs) noexcept {
			return Quaternion{ lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs };
		}
		friend inline constexpr Quaternion operator*(float lhs, const Quaternion& rhs) noexcept {
			return Quaternion{ lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w };
		}
		friend inline constexpr Vector3 operator*(const Quaternion& lhs, const Vector3& rhs) noexcept {
			return rhs + 2.0f * Vector3::Cross({ lhs.x, lhs.y, lhs.z }, Vector3::Cross({ lhs.x, lhs.y, lhs.z }, rhs) + lhs.w * rhs);
		}
		friend inline constexpr Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs) noexcept {
			return Quaternion{
				lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
				lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z,
				lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x,
				lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z };
		}
		friend inline constexpr Quaternion& operator*=(Quaternion& lhs, const Quaternion& rhs) noexcept {
			lhs = lhs * rhs;
			return lhs;
		}
		friend inline constexpr bool operator==(const Quaternion& lhs, const Quaternion& rhs) noexcept {
			return Dot(lhs, rhs) > 0.99999f;
		}
		friend inline constexpr bool operator!=(const Quaternion& lhs, const Quaternion& rhs) noexcept {
			return !(lhs == rhs);
		}
#pragma endregion
#pragma region メンバ関数
		// オイラー角（不安）
		inline Vector3 EulerAngle() const noexcept {
			Vector3 euler{};
			euler.x = std::atan2(2.0f * (w * x + y * z), 1.0f - 2.0f * (x * x + y * y));
			euler.y = std::asin(2.0f * (w * y - z * x));
			euler.z = std::atan2(2.0f * (w * z + x * y), 1.0f - 2.0f * (y * y + z * z));
			return euler;
		}
		// 長さの二乗
		inline constexpr float LengthSquare() const noexcept {
			return x * x + y * y + z * z + w * w;
		}
		// 長さ
		inline float Length() const noexcept {
			return std::sqrt(LengthSquare());
		}
		// 正規化
		inline Quaternion Normalized() const noexcept {
			return *this * (1.0f / Length());
		}
		// 回転角
		inline float Angle() const noexcept {
			return std::acos(w) * 2.0f;
		}
		// 回転軸
		inline Vector3 Axis() const noexcept {
			return Vector3{ x, y, z } *(1.0f / std::sin(std::acos(w)));
		}
		/// 共役
		inline constexpr Quaternion Conjugate() const noexcept {
			return Quaternion{ -x, -y, -z, w };
		}
		// 逆クォータニオン
		inline Quaternion Inverse() const noexcept {
			return Conjugate() * (1.0f / Length());
		}
#pragma endregion
#pragma region 静的関数
		// 内積
		static inline constexpr float Dot(const Quaternion& lhs, const Quaternion& rhs) noexcept {
			return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
		}
		// 線形補間
		static inline constexpr Quaternion Lerp(float t, const Quaternion& start, const Quaternion& end) noexcept {
			return Quaternion{
				start.x + t * (end.x - start.x),
				start.y + t * (end.y - start.y),
				start.z + t * (end.z - start.z),
				start.w + t * (end.w - start.w) };
		}
		// 球面線形補間
		static Quaternion Slerp(float t, const Quaternion& start, const Quaternion& end) noexcept;
#pragma endregion 
#pragma region メンバ変数
		float x, y, z, w;
#pragma endregion
	};

	// 44行列
	class Matrix4x4 {
	public:
#pragma region 定数
		static const Matrix4x4 identity;
#pragma endregion
#pragma region ファクトリ関数
		// 拡大縮小行列
		static inline Matrix4x4 MakeScaling(const Vector3& scale) noexcept {
			return {
				scale.x, 0.0f, 0.0f, 0.0f,
				0.0f, scale.y, 0.0f, 0.0f,
				0.0f, 0.0f, scale.z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f };
		}
		// X軸回りの回転行列
		static inline Matrix4x4 MakeRotationX(float rotate) noexcept {
			float c = std::cos(rotate);
			float s = std::sin(rotate);
			return {
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, c, s, 0.0f,
				0.0f, -s, c, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f };
		}
		// Y軸回りの回転行列
		static inline Matrix4x4 MakeRotationY(float rotate) noexcept {
			float s = std::sin(rotate);
			float c = std::cos(rotate);
			return {
				c,		0.0f,	-s,		0.0f,
				0.0f,	1.0f,	0.0f,	0.0f,
				s,		0.0f,	c,		0.0f,
				0.0f,	0.0f,	0.0f,	1.0f };
		}
		// Z軸回りの回転行列
		static inline Matrix4x4 MakeRotationZ(float rotate) noexcept {
			float s = std::sin(rotate);
			float c = std::cos(rotate);
			return {
				c,		s,		0.0f,	0.0f,
				-s,		c,		0.0f,	0.0f,
				0.0f,	0.0f,	1.0f,	0.0f,
				0.0f,	0.0f,	0.0f,	1.0f };
		}
		// XYZ順の回転行列
		static inline Matrix4x4 MakeRotationXYZ(const Vector3& rotate) noexcept {
			Vector3 s = { std::sin(rotate.x), std::sin(rotate.y), std::sin(rotate.z) };
			Vector3 c = { std::cos(rotate.x), std::cos(rotate.y), std::cos(rotate.z) };
			return {
				c.y * c.z,						c.y * s.z,						-s.y,		0.0f,
				s.x * s.y * c.z - c.x * s.z,	s.x * s.y * s.z + c.x * c.z,	s.x * c.y,	0.0f,
				c.x * s.y * c.z + s.x * s.z,	c.x * s.y * s.z - s.x * c.z,	c.x * c.y,	0.0f,
				0.0f,	0.0f,	0.0f,	1.0f };
		}
		// クォータニオンから回転行列
		static inline constexpr Matrix4x4 MakeRotation(const Quaternion& q) noexcept {
			float w2 = q.w * q.w, x2 = q.x * q.x, y2 = q.y * q.y, z2 = q.z * q.z;
			float wx = q.w * q.x, wy = q.w * q.y, wz = q.w * q.z;
			float xy = q.x * q.y, xz = q.x * q.z, yz = q.y * q.z;
			return {
				w2 + x2 - y2 - z2,	2.0f * (wz + xy),	2.0f * (xz - wy),	0.0f,
				2.0f * (xy - wz),	w2 - x2 + y2 - z2,	2.0f * (yz + wx),	0.0f,
				2.0f * (wy + xz),	2.0f * (-wx + yz),	w2 - x2 - y2 + z2,	0.0f,
				0.0f,				0.0f,				0.0f,				1.0f };
		}
		// 特定の方向に向ける回転行列
		static inline Matrix4x4 MakeLookRotation(const Vector3& direction, const Vector3& up = Vector3::up) noexcept {
			Vector3 z = direction.Normalized();
			Vector3 x = Vector3::Cross(up.Normalized(), z).Normalized();
			Vector3 y = Vector3::Cross(z, x);
			return {
				x.x, x.y, x.z, 0.0f,
				y.x, y.y, y.z, 0.0f,
				z.x, z.y, z.z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f };
		}
		// 平行移動行列
		static inline constexpr Matrix4x4 MakeTranslation(const Vector3& translate) noexcept {
			return {
				1.0f,		0.0f,		0.0f,		0.0f,
				0.0f,		1.0f,		0.0f,		0.0f,
				0.0f,		0.0f,		1.0f,		0.0f,
				translate.x,	translate.y,	translate.z,	1.0f };
		}
		// アフィン変換行列
		static inline Matrix4x4 MakeAffineTransform(const Vector3& scale, const Vector3& rotate, const Vector3& translate) noexcept {
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
					1.0f };
		}
		// アフィン変換行列
		static inline constexpr Matrix4x4 MakeAffine(const Vector3& scale, const Quaternion& rotate, const Vector3& translate) noexcept {
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
					1.0f };
		}
		// 透視投影行列
		static inline Matrix4x4 MakePerspectiveProjection(float fovY, float aspect, float nearZ, float farZ) noexcept {
			float s = 1.0f / std::tan(fovY * 0.5f);
			float a = farZ / (farZ - nearZ);
			return {
				s / aspect, 0.0f, 0.0f, 0.0f,
				0.0f, s, 0.0f, 0.0f,
				0.0f, 0.0f, a, 1.0f,
				0.0f, 0.0f, a * -nearZ, 0.0f };
		}
		// 平行投影行列
		static inline constexpr Matrix4x4 MakeOrthographicProjection(float width, float height, float nearZ, float farZ) noexcept {
			float zRange = farZ - nearZ;
			return {
				2.0f / width, 0.0f, 0.0f, 0.0f,
				0.0f, 2.0f / height, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f / zRange, 0.0f,
				0.0f, 0.0f, nearZ / -zRange, 1.0f };
		}
		// ビューポート行列
		static inline constexpr Matrix4x4 MakeViewport(float left, float top, float width, float height, float nearZ, float farZ) noexcept {
			float halfW = width / 2.0f;
			float halfh = height / 2.0f;
			return {
				halfW, 0.0f, 0.0f, 0.0f,
				0.0f, -halfh, 0.0f, 0.0f,
				0.0f, 0.0f, farZ - nearZ, 0.0f,
				left + halfW, top + halfh, nearZ, 1.0f };
		}
#pragma endregion
#pragma region コンストラクタ
		inline constexpr Matrix4x4() noexcept {
			*this = identity;
		}
		inline constexpr Matrix4x4(
			float _00, float _01, float _02, float _03,
			float _10, float _11, float _12, float _13,
			float _20, float _21, float _22, float _23,
			float _30, float _31, float _32, float _33) noexcept {
			m[0][0] = _00, m[0][1] = _01, m[0][2] = _02, m[0][3] = _03;
			m[1][0] = _10, m[1][1] = _11, m[1][2] = _12, m[1][3] = _13;
			m[2][0] = _20, m[2][1] = _21, m[2][2] = _22, m[2][3] = _23;
			m[3][0] = _30, m[3][1] = _31, m[3][2] = _32, m[3][3] = _33;
		}
#pragma endregion
#pragma region 演算子のオーバーロード
		friend inline constexpr Matrix4x4 operator*(const Matrix4x4& lhs, const Matrix4x4& rhs) noexcept {
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
		friend inline constexpr Vector3 operator*(const Vector3& lhs, const Matrix4x4& rhs) noexcept {
			return {
					lhs.x * rhs.m[0][0] + lhs.y * rhs.m[1][0] + lhs.z * rhs.m[2][0] + rhs.m[3][0],
					lhs.x * rhs.m[0][1] + lhs.y * rhs.m[1][1] + lhs.z * rhs.m[2][1] + rhs.m[3][1],
					lhs.x * rhs.m[0][2] + lhs.y * rhs.m[1][2] + lhs.z * rhs.m[2][2] + rhs.m[3][2] };
		}
		friend inline constexpr Vector4 operator*(const Vector4& lhs, const Matrix4x4& rhs) noexcept {
			return {
				lhs.x * rhs.m[0][0] + lhs.y * rhs.m[1][0] + lhs.z * rhs.m[2][0] + lhs.w * rhs.m[3][0],
				lhs.x * rhs.m[0][1] + lhs.y * rhs.m[1][1] + lhs.z * rhs.m[2][1] + lhs.w * rhs.m[3][1],
				lhs.x * rhs.m[0][2] + lhs.y * rhs.m[1][2] + lhs.z * rhs.m[2][2] + lhs.w * rhs.m[3][2],
				lhs.x * rhs.m[0][3] + lhs.y * rhs.m[1][3] + lhs.z * rhs.m[2][3] + lhs.w * rhs.m[3][3] };
		}
		friend inline constexpr Matrix4x4 operator*(float lhs, const Matrix4x4& rhs) noexcept {
			return {
				lhs * rhs.m[0][0], lhs * rhs.m[0][1], lhs * rhs.m[0][2], lhs * rhs.m[0][3],
				lhs * rhs.m[1][0], lhs * rhs.m[1][1], lhs * rhs.m[1][2], lhs * rhs.m[1][3],
				lhs * rhs.m[2][0], lhs * rhs.m[2][1], lhs * rhs.m[2][2], lhs * rhs.m[2][3],
				lhs * rhs.m[3][0], lhs * rhs.m[3][1], lhs * rhs.m[3][2], lhs * rhs.m[3][3] };
		}
		friend inline constexpr Matrix4x4 operator*(const Matrix4x4& lhs, float rhs) noexcept {
			return {
				lhs.m[0][0] * rhs, lhs.m[0][1] * rhs, lhs.m[0][2] * rhs, lhs.m[0][3] * rhs,
				lhs.m[1][0] * rhs, lhs.m[1][1] * rhs, lhs.m[1][2] * rhs, lhs.m[1][3] * rhs,
				lhs.m[2][0] * rhs, lhs.m[2][1] * rhs, lhs.m[2][2] * rhs, lhs.m[2][3] * rhs,
				lhs.m[3][0] * rhs, lhs.m[3][1] * rhs, lhs.m[3][2] * rhs, lhs.m[3][3] * rhs };
		}
		friend inline constexpr Matrix4x4& operator*=(Matrix4x4& lhs, const Matrix4x4& rhs) noexcept {
			lhs = lhs * rhs;
			return lhs;
		}
#pragma endregion
#pragma region メンバ関数
		// ベクトルに回転を適用
		inline constexpr Vector3 ApplyRotation(const Vector3& vector) const noexcept {
			return {
				vector.x * m[0][0] + vector.y * m[1][0] + vector.z * m[2][0],
				vector.x * m[0][1] + vector.y * m[1][1] + vector.z * m[2][1],
				vector.x * m[0][2] + vector.y * m[1][2] + vector.z * m[2][2] };
		}
		// ベクトルに変換を適用しWで割る
		inline constexpr Vector3 ApplyTransformWDivide(const Vector3& vector) const noexcept {
			Vector3 result{
				vector.x * m[0][0] + vector.y * m[1][0] + vector.z * m[2][0] + m[3][0],
				vector.x * m[0][1] + vector.y * m[1][1] + vector.z * m[2][1] + m[3][1],
				vector.x * m[0][2] + vector.y * m[1][2] + vector.z * m[2][2] + m[3][2] };
			float w = vector.x * m[0][3] + vector.y * m[1][3] + vector.z * m[2][3] + m[3][3];
			result /= w;
			return result;
		}
		// 行
		inline constexpr Matrix4x4& Row(size_t i, const Vector4& v) noexcept {
			m[i][0] = v.x, m[i][1] = v.y, m[i][2] = v.z, m[i][3] = v.w;
			return *this;
		}
		// 行
		inline constexpr Vector4 Row(size_t i) const noexcept {
			return { m[i][0], m[i][1], m[i][2], m[i][3] };
		}
		// 列
		inline constexpr Matrix4x4& Column(size_t i, const Vector4& v) noexcept {
			m[0][i] = v.x, m[1][i] = v.y, m[2][i] = v.z, m[3][i] = v.w;
			return *this;
		}
		// 列
		inline constexpr Vector4 Column(size_t i) const noexcept {
			return { m[0][i], m[1][i], m[2][i], m[3][i] };
		}
		// X軸
		inline constexpr Vector3 XAxis() const noexcept {
			return { m[0][0], m[0][1], m[0][2] };
		}
		// Y軸
		inline constexpr Vector3 YAxis() const noexcept {
			return { m[1][0], m[1][1], m[1][2] };
		}
		// Z軸
		inline constexpr Vector3 GetZAxis() const noexcept {
			return { m[2][0], m[2][1], m[2][2] };
		}
		// 平行移動成分
		inline constexpr Vector3 Translate() const noexcept {
			return { m[3][0], m[3][1], m[3][2] };
		}
		// 行列式
		float Determinant() noexcept;
		// 余因子行列
		Matrix4x4 Adjugate() noexcept;
		// 逆行列
		inline Matrix4x4 Inverse() noexcept {
			return 1.0f / Determinant() * Adjugate();
		}
		// 転置行列
		inline constexpr Matrix4x4 Transpose() noexcept {
			return {
				m[0][0], m[1][0], m[2][0], m[3][0],
				m[0][1], m[1][1], m[2][1], m[3][1],
				m[0][2], m[1][2], m[2][2], m[3][2],
				m[0][3], m[1][3], m[2][3], m[3][3] };
		}
#pragma endregion
#pragma region メンバ変数
		float m[4][4];
#pragma endregion
	};

} // namespace AomiEngine

