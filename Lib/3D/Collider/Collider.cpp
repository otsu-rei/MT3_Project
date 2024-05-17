#include "Collider.h"

////////////////////////////////////////////////////////////////////////////////////////////
// methods
////////////////////////////////////////////////////////////////////////////////////////////

Vector3f Project(const Vector3f& v1, const Vector3f& v2) {
	Vector3f result;

	Vector3f normalv2 = Vector::Normalize(v2);
	result = normalv2 * Vector::Dot(v1, normalv2);

	return result;
}

Vector3f ClosestPoint(const Vector3f& point, const Segment& segment) {
	Vector3f result;

	result = segment.origin + Project(point - segment.origin, segment.diff);

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Collider namespace methods
////////////////////////////////////////////////////////////////////////////////////////////

bool Collider::SphereTo(const Sphere& a, const Sphere& b) {
	float distance = Vector::Length(a.center - b.center);

	if (distance <= a.radius + b.radius) {
		return true;
	}

	return false;
}

bool Collider::PlaneToSphere(const Plane& a, const Sphere& b) {

	Vector3f center = b.center - (a.normal * a.distance);
	float distance = Vector::Length(center);

	if (distance <= b.radius) {
		return true;
	}

	return false;
}
