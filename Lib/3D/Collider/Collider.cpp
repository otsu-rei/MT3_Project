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

bool Collider::PlaneToSphere(const Plane& plane, const Sphere& sphere) {

	float distance = Vector::Dot(plane.normal, sphere.center) - plane.distance;
	distance = distance < 0.0f ? -distance : distance; // 絶対値

	if (distance <= sphere.radius) {
		return true;
	}

	return false;
}

bool Collider::PlaneToLine(const Plane& plane, const Line& line) {
	float dot = Vector::Dot(line.diff, plane.normal);

	if (dot == 0.0f) {
		return false;
	}

	return true;
}

bool Collider::PlaneToSegment(const Plane& plane, const Segment& segment) {
	float dot = Vector::Dot(segment.diff, plane.normal);

	if (dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - Vector::Dot(segment.origin, plane.normal)) / dot;

	if (t < 0.0f || t > 1.0f) {
		return false; // segment(線分)なのでtが0~1
	}

	return true;
}
