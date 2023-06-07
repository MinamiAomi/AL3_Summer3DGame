#pragma once
#include "MathUtils.hpp"

namespace Math {

	inline float Lerp(float t, float start, float end) {
		return start + t * (end - start);
	}

}

