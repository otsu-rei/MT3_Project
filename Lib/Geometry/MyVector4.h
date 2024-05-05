#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
#include <cstdint>

////////////////////////////////////////////////////////////////////////////////////////////
// Vector4 class
////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
class Vector4 {
public:

	union {
		T x, r;
	};

	union {
		T y, g;
	};

	union {
		T z, b;
	};

	union {
		T w, a;
	};

};

//-----------------------------------------------------------------------------------------
// using
//-----------------------------------------------------------------------------------------
using Vector4f = Vector4<float>;

////////////////////////////////////////////////////////////////////////////////////////////
// Color namespace
////////////////////////////////////////////////////////////////////////////////////////////
namespace Color {

	uint32_t ChangeColorCode(const Vector4f& color);

}