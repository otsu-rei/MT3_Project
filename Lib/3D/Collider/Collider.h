#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
#include <MyVector3.h>
#include <MyMath.h>

////////////////////////////////////////////////////////////////////////////////////////////
// Line Structure
////////////////////////////////////////////////////////////////////////////////////////////
struct Line {
	Vector3f origin; //!< 始点
	Vector3f diff;   //!< 終点ベクトルへの差分
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


}

