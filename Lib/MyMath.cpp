#include "MyMath.h"

void Test::DrawShadow(
	const Vector3f& localA, const Vector3f& localB, const Vector3f& localC,
	float bottomY, const Camera3D& camera, const Light& light, Polygon3& polygon) {

	Vector3f resultPos[3];

	Vector3f localPos[3] = {
		localA,
		localB,
		localC,
	};

	for (int i = 0; i < 3; i++) {
		Vector3f direction = Vector::CalculateDirection(light.GetPos(), localPos[i]);
		Vector::Normalize(direction);

		if (direction.y == 0.0f) { return; }

		float t = (localPos[i].y - bottomY) / direction.y;
		if (t > 0) { return; }

		resultPos[i] = {
			localPos[i].x - (direction.x * t),
			bottomY,
			localPos[i].z - (direction.z * t),
		};
	}

	polygon.DrawPolygon3(
		resultPos[0], resultPos[1], resultPos[2],
		Matrix4x4::MakeAffine({1.0f, 1.0f, 1.0f}, {0.0f}, {0.0f}),
		0x20202030,
		camera
	);
}
