#pragma once

#include <cmath>

namespace AomiEngine {

	class Vector4 {
	public:
		float x, y, z, w;

		static const Vector4 zero;
		static const Vector4 one;

		constexpr Vector4() noexcept : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
		constexpr Vector4(float x, float y, float z, float w) noexcept : x(x), y(y), z(z), w(w) {}
		constexpr Vector4(const Vector3& xyz, float w) noexcept : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
		constexpr explicit Vector4(float xyzw) noexcept : x(xyzw), y(xyzw), z(xyzw), w(xyzw) {}

		constexpr float& operator[](std::size_t i) { return *(&x + i); }
		constexpr const float& operator[](size_t i) const { return *(&x + i); }
		constexpr explicit operator Vector3() const { return { x, y, z }; }

		friend inline Vector4 operator+(const Vector4& v);
		friend inline Vector4 operator-(const Vector4& v);
		friend inline Vector4 operator+(const Vector4& v1, const Vector4& v2);
		friend inline Vector4 operator-(const Vector4& v1, const Vector4& v2);
		friend inline Vector4 operator*(const Vector4& v, float s);
		friend inline Vector4 operator*(float s, const Vector4& v);
		friend inline Vector4& operator+=(Vector4& v1, const Vector4& v2);
		friend inline Vector4& operator-=(Vector4& v1, const Vector4& v2);
		friend inline Vector4& operator*=(Vector4& v, float s);
		friend inline bool operator==(const Vector4& v1, const Vector4& v2);
		friend inline bool operator!=(const Vector4& v1, const Vector4& v2);


		__declspec(property(get = GetXYZ, put = SetXYZ)) Vector3 xyz;
		inline Vector4& SetXYZ(const Vector3& xyz);
		inline Vector3 GetXYZ() const;

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
		__declspec(property(get = Normalized)) Vector4 normalized;
		inline Vector4 Normalized() const;

		/// <summary>
		/// 内積
		/// </summary>
		/// <param name="lhs"></param>
		/// <param name="rhs"></param>
		/// <returns></returns>
		static inline float Dot(const Vector4& lhs, const Vector4& rhs);
		/// <summary>
		/// それぞれの要素を掛ける
		/// </summary>
		/// <param name="lhs"></param>
		/// <param name="rhs"></param>
		/// <returns></returns>
		static inline Vector4 Scale(const Vector4& lhs, const Vector4& rhs);
		/// <summary>
		/// 正射影ベクトル
		/// </summary>
		/// <param name="base"></param>
		/// <param name="direction"></param>
		/// <returns></returns>
		static inline Vector4 Project(const Vector4& base, const Vector4& direction);
		/// <summary>
		/// それぞれの要素の最小
		/// </summary>
		/// <param name="lhs"></param>
		/// <param name="rhs"></param>
		/// <returns></returns>
		static inline Vector4 Min(const Vector4& lhs, const Vector4& rhs);
		/// <summary>
		/// それぞれの要素の最大
		/// </summary>
		/// <param name="lhs"></param>
		/// <param name="rhs"></param>
		/// <returns></returns>
		static inline Vector4 Max(const Vector4& lhs, const Vector4& rhs);
		/// <summary>
		/// 線形補間
		/// </summary>
		/// <param name="t"></param>
		/// <param name="start"></param>
		/// <param name="end"></param>
		/// <returns></returns>
		static inline Vector4 Lerp(float t, const Vector4& start, const Vector4& end);
	};

} // namespace AomiEngine

#include "Vector4_inline.hpp"