#pragma once

//*****************************************************************************************
// Vectorどうしでの比較を行うoperatorと関数
//*****************************************************************************************

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
#include <MyVector4.h> 
#include <MyVector3.h>
#include <MyVector2.h>

////////////////////////////////////////////////////////////////////////////////////////////
// using
////////////////////////////////////////////////////////////////////////////////////////////
using Flag4 = Vector4<bool>;
using Flag3 = Vector3<bool>;

////////////////////////////////////////////////////////////////////////////////////////////
// operator
////////////////////////////////////////////////////////////////////////////////////////////

/* == */

inline Flag4 operator==(const Vector4f& a, const Vector4f& b) {
	return { a.x == b.x, a.y == b.y, a.z == b.z, a.w == b.w };
}

inline Flag3 operator==(const Vector3f& a, const Vector3f& b) {
	return { a.x == b.x, a.y == b.y, a.z == b.z };
}

/* != */

inline Flag4 operator!=(const Vector4f& a, const Vector4f& b) {
	return { a.x != b.x, a.y != b.y, a.z != b.z, a.w != b.w };
}

inline Flag3 operator!=(const Vector3f& a, const Vector3f& b) {
	return { a.x != b.x, a.y != b.y, a.z != b.z };
}

/* >= */

inline Flag4 operator>=(const Vector4f& a, const Vector4f& b) {
	return { a.x >= b.x, a.y >= b.y, a.z >= b.z, a.w >= b.w };
}

inline Flag3 operator>=(const Vector3f& a, const Vector3f& b) {
	return { a.x >= b.x, a.y >= b.y, a.z >= b.z };
}

/* > */

inline Flag4 operator>(const Vector4f& a, const Vector4f& b) {
	return { a.x > b.x, a.y > b.y, a.z > b.z, a.w > b.w };
}

inline Flag3 operator>(const Vector3f& a, const Vector3f& b) {
	return { a.x > b.x, a.y > b.y, a.z > b.z };
}

/* <= */

inline Flag4 operator<=(const Vector4f& a, const Vector4f& b) {
	return { a.x <= b.x, a.y <= b.y, a.z <= b.z, a.w <= b.w };
}

inline Flag3 operator<=(const Vector3f& a, const Vector3f& b) {
	return { a.x <= b.x, a.y <= b.y, a.z <= b.z };
}

/* < */

inline Flag4 operator<(const Vector4f& a, const Vector4f& b) {
	return { a.x < b.x, a.y < b.y, a.z < b.z, a.w < b.w };
}

inline Flag3 operator<(const Vector3f& a, const Vector3f& b) {
	return { a.x < b.x, a.y < b.y, a.z < b.z };
}

////////////////////////////////////////////////////////////////////////////////////////////
// methods
////////////////////////////////////////////////////////////////////////////////////////////

/* All */

inline bool All(const Flag4& f) {
	return f.x && f.y && f.z && f.w;
}

inline bool All(const Flag3& f) {
	return f.x && f.y && f.z;
}

/* Any */

inline bool Any(const Flag4& f) {
	return f.x || f.y || f.z || f.w;
}

inline bool Any(const Flag3& f) {
	return f.x || f.y || f.z;
}