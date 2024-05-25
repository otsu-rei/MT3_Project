#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
#include <MyVector3.h>
#include <MyMath.h>
#include <imgui.h>
#include <VectorComparison.h>

////////////////////////////////////////////////////////////////////////////////////////////
// Object Structure
////////////////////////////////////////////////////////////////////////////////////////////

struct Line {
	Vector3f origin; //!< 始点
	Vector3f diff;   //!< 終点ベクトルへの差分

	void SetOnImGui() {
		ImGui::DragFloat3("origin", &origin.x, 0.02f);
		ImGui::DragFloat3("diff",   &diff.x,   0.02f);
	}
};

struct Sphere {
	Vector3f center;
	float radius;

	void SetOnImGui() {
		ImGui::DragFloat3("center", &center.x, 0.02f);
		ImGui::DragFloat("radius",  &radius,   0.02f);
	}
};

struct Plane {
	Vector3f normal; //!< 法線
	float distance;  //!< 距離

	void SetOnImGui() {
		ImGui::DragFloat3("normal",  &normal.x, 0.02f);
		normal = Vector::Normalize(normal);

		ImGui::DragFloat("distance", &distance, 0.02f);
	}
};

struct Triangle {
	Vector3f virtices[3];

	void SetOnImGui() {
		ImGui::DragFloat3("virtices[0]", &virtices[0].x, 0.02f);
		ImGui::DragFloat3("virtices[1]", &virtices[1].x, 0.02f);
		ImGui::DragFloat3("virtices[2]", &virtices[2].x, 0.02f);
	}
};

struct AABB {
	Vector3f min;
	Vector3f max;

	void SetOnImGui() {
		ImGui::DragFloat3("min", &min.x, 0.02f);
		ImGui::DragFloat3("max", &max.x, 0.02f);

		// clamps
		min.x = (std::min)(min.x, max.x);
		max.x = (std::max)(min.x, max.x);

		min.y = (std::min)(min.y, max.y);
		max.y = (std::max)(min.y, max.y);

		min.z = (std::min)(min.z, max.z);
		max.z = (std::max)(min.z, max.z);
	}
};

////////////////////////////////////////////////////////////////////////////////////////////
// using
////////////////////////////////////////////////////////////////////////////////////////////
using Ray     = Line;
using Segment = Line;

////////////////////////////////////////////////////////////////////////////////////////////
// methods
////////////////////////////////////////////////////////////////////////////////////////////

Vector3f Project(const Vector3f& v1, const Vector3f& v2);
Vector3f ClosestPoint(const Vector3f& point, const Segment& segment);

////////////////////////////////////////////////////////////////////////////////////////////
// Collider namespace
////////////////////////////////////////////////////////////////////////////////////////////
namespace Collider {

	bool SphereTo(const Sphere& a, const Sphere& b);
	bool PlaneToSphere(const Plane& plane, const Sphere& sphere);
	bool PlaneToLine(const Plane& plane, const Line& line);
	bool PlaneToSegment(const Plane& plane, const Segment& segment);
	bool SegmentToTriangle(const Segment& segment, const Triangle& triangle);

}

