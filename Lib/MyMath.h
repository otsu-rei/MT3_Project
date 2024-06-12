#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
#include <MyVector3.h>
#include <MyMatrix4x4.h>

////////////////////////////////////////////////////////////////////////////////////////////
// Vector namespace
////////////////////////////////////////////////////////////////////////////////////////////
namespace Vector {

	float Length(const Vector3f& v);

	float Dot(const Vector3f& x, const Vector3f& y);

	Vector3f Normalize(const Vector3f& v);

	Vector3f Cross(const Vector3f& x, const Vector3f& y);

	Vector3f Reflect(const Vector3f& i, const Vector3f& n);

	Vector3f Lerp(const Vector3f& x, const Vector3f& y, float s);

	Vector3f Perpendicular(const Vector3f& v);

	Vector3f Clamp(const Vector3f& v, const Vector3f& min, const Vector3f& max);

	Vector3f Max(const Vector3f& a, const Vector3f& b);

}

////////////////////////////////////////////////////////////////////////////////////////////
// Matrix namespace
////////////////////////////////////////////////////////////////////////////////////////////
namespace Matrix {

	Matrix4x4 MakeScale(const Vector3f& scale);

	Matrix4x4 MakeRotateX(float radian);
	Matrix4x4 MakeRotateY(float radian);
	Matrix4x4 MakeRotateZ(float radian);
	Matrix4x4 MakeRotate(const Vector3f& rotate);

	Matrix4x4 MakeRotateEulerX(float radian);
	Matrix4x4 MakeRotateEulerY(float radian);
	Matrix4x4 MakeRotateEulerZ(float radian);
	Matrix4x4 MakeRotateEuler(const Vector3f& rotate);

	Matrix4x4 MakeTranslate(const Vector3f& translate);

	Matrix4x4 MakeAffine(const Vector3f& scale, const Vector3f& rotate, const Vector3f& translate);

	Matrix4x4 Inverse(const Matrix4x4& m);

	Matrix4x4 Transpose(const Matrix4x4& m);

	// graphics matrix //
	Matrix4x4 MakePerspectiveFov(float fovY, float aspectRatio, float nearClip, float farClip);
	Matrix4x4 MakeOrthographic(float left, float top, float right, float bottom, float nearZ, float farZ);

	Matrix4x4 MakeViewport(float left, float top, float width, float height, float minDepth, float maxDepth);

	Vector3f Transform(const Vector3f& vector, const Matrix4x4& matrix);
}

////////////////////////////////////////////////////////////////////////////////////////////
// methods
////////////////////////////////////////////////////////////////////////////////////////////

float Saturate(float v);