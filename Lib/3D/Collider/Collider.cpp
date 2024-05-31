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

bool Collider::SegmentToTriangle(const Segment& segment, const Triangle& triangle) {

	Vector3f n = Vector::Cross(triangle.virtices[1] - triangle.virtices[0], triangle.virtices[2] - triangle.virtices[1]);
	n = Vector::Normalize(n);

	float dot = Vector::Dot(segment.diff, n);

	if (dot == 0.0f) {
		return false; // 平行なので
	}

	float d = Vector::Dot(triangle.virtices[0], n);
	d = d < 0.0f ? d * -1 : d;

	float t = d - Vector::Dot(segment.origin, n) / dot;

	if (t < 0.0f || t > 1.0f) {
		return false; // segment(線分)なのでtが0~1
	}

	Vector3f p = segment.origin + segment.diff * t;

	for (int i = 0; i < 3; ++i) {
		int next = (i + 1) % 3;
		Vector3f cross = Vector::Cross(triangle.virtices[next] - triangle.virtices[i], p - triangle.virtices[next]);

		if (Vector::Dot(cross, n) < 0.0f) {
			return false;
		}
	}

	return true;
}

bool Collider::AABBTo(const AABB& a, const AABB& b) {
	if (All(a.min <= b.max) && All(a.max >= b.min)) {
		return true;
	}

	return false;
}
