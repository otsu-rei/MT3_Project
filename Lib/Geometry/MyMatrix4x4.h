#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
#include <algorithm>
#include <cmath>
#include <cassert>
#include <MyVector3.h>

////////////////////////////////////////////////////////////////////////////////////////////
// Matrix4x4 class
////////////////////////////////////////////////////////////////////////////////////////////
class Matrix4x4 {
public:

	float m[4][4];

	static Matrix4x4 MakeIdentity();

	//=========================================================================================
	// compound assignment operator
	//=========================================================================================

	/* Add */
	Matrix4x4& operator+=(const Matrix4x4& mat) {
		for (int row = 0; row < 4; row++) {
			for (int column = 0; column < 4; column++) {
				m[row][column] += mat.m[row][column];
			}
		}

		return *this;
	}

	/* Subtract */
	Matrix4x4& operator-=(const Matrix4x4& mat) {
		for (int row = 0; row < 4; row++) {
			for (int column = 0; column < 4; column++) {
				m[row][column] -= mat.m[row][column];
			}
		}

		return *this;
	}

	/* Multiply */
	Matrix4x4& operator*=(const Matrix4x4& mat) {
		Matrix4x4 result = { 0.0f };

		for (int row = 0; row < 4; row++) {
			for (int column = 0; column < 4; column++) {
				for (int i = 0; i < 4; i++) {
					result.m[row][column] += m[row][i] * mat.m[i][column];
				}
			}
		}

		std::memcpy(this, &result, sizeof(Matrix4x4));
		return *this;
	}

};

////////////////////////////////////////////////////////////////////////////////////////////
// operators
////////////////////////////////////////////////////////////////////////////////////////////
//=========================================================================================
// binary operator
//=========================================================================================

/* Add */
inline Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;

	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			result.m[row][column] = m1.m[row][column] + m2.m[row][column];
		}
	}

	return result;
}

/* Subtract */
inline Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;

	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			result.m[row][column] = m1.m[row][column] - m2.m[row][column];
		}
	}

	return result;
}

/* Multiply */
inline Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = { 0.0f };

	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			for (int i = 0; i < 4; i++) {
				result.m[row][column] += m1.m[row][i] * m2.m[i][column];
			}
		}
	}

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////
// MatrixNovice namespace
////////////////////////////////////////////////////////////////////////////////////////////
namespace MatrixNovice {

	static const int kRowHeight   = 20;
	static const int kColumnWidth = 60;

	void ScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);

}