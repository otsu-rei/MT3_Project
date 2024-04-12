#pragma once

// Library > GeometryClass //
#include "MyVector2.h"

template <typename Type>
class Vertex4 {
public:
	Vector2 <Type>leftTop;
	Vector2 <Type>rightTop;
	Vector2 <Type>leftBottom;
	Vector2 <Type>rightBottom;
};

using Vertex4f = Vertex4 <float>;

