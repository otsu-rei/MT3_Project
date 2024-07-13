#include "MyVector4.h"

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//c++
#include <cmath>

////////////////////////////////////////////////////////////////////////////////////////////
// anonymose namespacae
////////////////////////////////////////////////////////////////////////////////////////////

namespace {

	//! @brief vector結果を出力するための構造体
	struct float3 {
		float x, y, z;
	};

	//! @brief QuartanionのVector部分のdot演算用関数
	float Dot(const Quaternion& q, const Quaternion& r) {
		return q.x * r.x + q.y * r.y + q.z * r.z;
	}

	//! @brief QuartanionのVector部分のcross演算用関数
	float3 Cross(const Quaternion& q, const Quaternion& r) {
		return {
			q.y * r.z - q.z * r.y,
			q.z * r.x - q.x * r.z,
			q.x * r.y - q.y * r.x
		};
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// Quaternion class operator
////////////////////////////////////////////////////////////////////////////////////////////

Quaternion Quaternion::Identity() {
	return { 0.0f, 0.0f, 0.0f, 1.0f };
}

Quaternion& Quaternion::operator*=(const Quaternion& q) {

	Quaternion result;

	float3 cross = Cross(*this, q);
	float dot    = Dot(*this, q);

	result.x = cross.x + q.w * x + w * q.x;
	result.y = cross.y + q.w * y + w * q.y;
	result.z = cross.z + q.w * z + w * q.z;
	result.w = w * q.w - dot;

	*this = result;
	return *this;
}

Quaternion operator*(const Quaternion& q, const Quaternion& r) {
	
	Quaternion result;

	float3 cross = Cross(q, r);
	float dot = Dot(q, r);

	result.x = cross.x + r.w * q.x + q.w * r.x;
	result.y = cross.y + r.w * q.y + q.w * r.y;
	result.z = cross.z + r.w * q.z + q.w * r.z;
	result.w = q.w * r.w - dot;

	return result;
}

Quaternion Conjugation(const Quaternion& q) {
	return { -q.x, -q.y, -q.z, q.w };
}

float Norm(const Quaternion& q) {
	return std::sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
}

Quaternion Normalize(const Quaternion& q) {
	float norm = Norm(q);

	return { q.x / norm, q.y / norm, q.z / norm, q.w / norm };
}

Quaternion Inverse(const Quaternion& q) {

	Quaternion conj = Conjugation(q);
	float norm2 = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;

	return { conj.x / norm2, conj.y / norm2, conj.z / norm2, conj.w / norm2 };
}
