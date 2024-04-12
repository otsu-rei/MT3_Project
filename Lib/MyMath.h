#pragma once

#include "Camera3D.h"
#include "Polygon3.h"

namespace Test {

	void DrawShadow(
		const Vector3f& localA, const Vector3f& localB, const Vector3f& localC,
		float bottomY, const Camera3D& camera, const Light& light, Polygon3& polygon
	);

}


