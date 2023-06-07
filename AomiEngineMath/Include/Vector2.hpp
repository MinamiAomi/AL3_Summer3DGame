#pragma once

#include <cmath>
#include <algorithm>

namespace AomiEngine {

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
		constexpr inline Vector2() noexcept : x(0.0f), y(0.0f) {}
		constexpr inline Vector2(float x, float y) noexcept : x(x), y(y) {}
		constexpr explicit inline Vector2(float xy) noexcept : x(xy), y(xy) {}
#pragma endregion
#pragma region 演算子のオーバーロード
		constexpr inline float& operator[](size_t i) noexcept {
			return *(&x + i);
		}
		constexpr inline const float& operator[](size_t i) const noexcept {
			return *(&x + i);
		}

		// constexpr explicit operator Vector3() const;

		friend constexpr inline Vector2 operator+(const Vector2& rhs) noexcept {
			return rhs;
		}
		friend constexpr inline Vector2 operator-(const Vector2& rhs) noexcept {
			return { -rhs.x, -rhs.y };
		}
		friend constexpr inline Vector2 operator+(const Vector2& lhs, const Vector2& rhs) noexcept {
			return { lhs.x + rhs.x, lhs.y + rhs.y };
		}
		friend constexpr inline Vector2 operator-(const Vector2& lhs, const Vector2& rhs) noexcept {
			return { lhs.x - rhs.x, lhs.y - rhs.y };
		}
		friend constexpr inline Vector2 operator*(const Vector2& lhs, float rhs) noexcept {
			return { lhs.x * rhs, lhs.y * rhs };
		}
		friend constexpr inline Vector2 operator*(float lhs, const Vector2& rhs) noexcept {
			return { lhs * rhs.x, lhs * rhs.y };
		}
		friend constexpr inline Vector2 operator/(const Vector2& lhs, float rhs) noexcept {
			return lhs * (1.0f / rhs);
		}
		friend constexpr inline Vector2& operator+=(Vector2& lhs, const Vector2& rhs) noexcept {
			lhs = lhs + rhs;
			return lhs;
		}
		friend constexpr inline Vector2& operator-=(Vector2& lhs, const Vector2& rhs) noexcept {
			lhs = lhs - rhs;
			return lhs;
		}
		friend constexpr inline Vector2& operator*=(Vector2& lhs, float rhs) noexcept {
			lhs = lhs * rhs;
			return lhs;
		}
		friend constexpr inline Vector2& operator/=(Vector2& lhs, float rhs) noexcept {
			lhs = lhs / rhs;
			return lhs;
		}
		friend constexpr inline bool operator==(const Vector2& lhs, const Vector2& rhs) noexcept {
			return lhs.x == rhs.x && lhs.y == rhs.y;
		}
		friend constexpr inline bool operator!=(const Vector2& lhs, const Vector2& rhs) noexcept {
			return !(lhs == rhs);
		}
#pragma endregion
#pragma region メンバ関数
		/// 長さの二乗
		constexpr inline float LengthSquare() const noexcept {
			return x * x + y * y;
		}
		/// 長さ
		constexpr inline float Length() const noexcept {
			return std::sqrt(LengthSquare());
		}
		/// 正規化
		constexpr Vector2 Normalized() const noexcept {
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
		static inline float Dot(const Vector2& lhs, const Vector2& rhs) {
			return lhs.x * rhs.x + lhs.y * rhs.y;
		}
		// 外積
		static inline float Cross(const Vector2& lhs, const Vector2& rhs) {
			return lhs.x * rhs.y - lhs.y * rhs.x;
		}
		// 垂直
		static inline Vector2 Perpendicular(const Vector2& direction) {
			return { -direction.y, direction.x };
		}
		// それぞれの要素を乗算
		static inline Vector2 Scale(const Vector2& lhs, const Vector2& rhs) {
			return { lhs.x * rhs.x, lhs.y * rhs.y };
		}
		// 正射影
		static inline Vector2 Project(const Vector2& base, const Vector2& direction) {
			Vector2 normV2 = direction.Normalized();
			return Dot(base, normV2) * normV2;
		}
		// 反射
		static inline Vector2 Reflecte(const Vector2& direction, const Vector2& normal) {
			Vector2 normN = normal.Normalized();
			return Dot(normN, -direction) * 2.0f * normN + direction;
		}
		// それぞれの要素の最小
		static inline Vector2 Min(const Vector2& lhs, const Vector2& rhs) {
			return { std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y) };
		}
		// それぞれの要素の最大											  
		static inline Vector2 Max(const Vector2& lhs, const Vector2& rhs) {
			return { std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y) };
		}
		// 線形補間
		static inline Vector2 Lerp(float t, const Vector2& start, const Vector2& end) {
			return start + t * (end - start);
		}
		// 球面線形補間
		static Vector2 Slerp(float t, const Vector2& start, const Vector2& end);
		// Catmull_Romスプライン曲線
		static Vector2 CatmullRomSpline(float t, const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3);
		// 二次ベジェ曲線
		static Vector2 QuadraticBezierCurve(float t, const Vector2& p0, const Vector2& p1, const Vector2& p2);
		// 三次ベジェ曲線
		static Vector2 CubicBezierCurve(float t, const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3);
#pragma endregion
#pragma region 変数
		float x, y;
#pragma endregion
	};

}
