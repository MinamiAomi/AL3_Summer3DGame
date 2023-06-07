#include "Quaternion.h"

const Quaternion Quaternion::identity{ 0.0f,0.0f,0.0f,1.0f };

Quaternion Quaternion::Slerp(float t, const Quaternion& start, const Quaternion& end) {
	Quaternion s = start;
	float dot = Dot(start, end);
	// q1, q2が反対向きの場合
	if (dot < 0) {
		s.x = -s.x;
		s.y = -s.y;
		s.z = -s.z;
		s.w = -s.w;
		dot = -dot;
	}
	// 球面線形補間の計算
	float theta = std::acos(dot);
	return (std::sin((1.0f - t) * theta) * s + std::sin(t * theta) * end) * (1.0f / std::sin(theta));
}