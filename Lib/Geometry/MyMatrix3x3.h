#pragma once
#include <Novice.h>
#include <cmath>
#include <assert.h>

// Library > GeometryClass //
#include "MyVector2.h"

class Matrix3x3 {
private:

	static const int kRowHeight = 20;
	static const int kColomWidth = 50;

public:

	static const int kMaxRow = 3;
	static const int kMaxColumn = 3;

	float m[kMaxRow][kMaxColumn];

	/// operator overload ///
	/* Add */
	Matrix3x3 operator+(const Matrix3x3& other) const {
		Matrix3x3 result;

		for (int row = 0; row < kMaxRow; row++) {
			for (int column = 0; column < kMaxColumn; column++) {
				result.m[row][column] = m[row][column] + other.m[row][column];
			}
		}

		return result;
	}
	void operator+=(const Matrix3x3& other) {
		for (int row = 0; row < kMaxRow; row++) {
			for (int column = 0; column < kMaxColumn; column++) {
				m[row][column] += other.m[row][column];
			}
		}
	}

	/* Subtruct */
	Matrix3x3 operator-(const Matrix3x3& other) const {
		Matrix3x3 result;

		for (int row = 0; row < kMaxRow; row++) {
			for (int column = 0; column < kMaxColumn; column++) {
				result.m[row][column] = m[row][column] - other.m[row][column];
			}
		}

		return result;
	}
	void operator-=(const Matrix3x3& other) {
		for (int row = 0; row < kMaxRow; row++) {
			for (int column = 0; column < kMaxColumn; column++) {
				m[row][column] -= other.m[row][column];
			}
		}
	}

	/* Multiply */
	Matrix3x3 operator*(const Matrix3x3& other) const {
		Matrix3x3 result = { 0.0f };

		for (int row = 0; row < kMaxRow; row++) {
			for (int column = 0; column < kMaxColumn; column++) {
				for (int i = 0; i < 3; i++) {
					result.m[row][column] += m[row][i] * other.m[i][column];
				}
			}
		}

		return result;
	}
	void operator*=(const Matrix3x3& other) {
		Matrix3x3 result = { 0.0f };

		for (int row = 0; row < kMaxRow; row++) {
			for (int column = 0; column < kMaxColumn; column++) {
				for (int i = 0; i < 3; i++) {
					result.m[row][column] += m[row][i] * other.m[i][column];
				}
			}
		}


		for (int row = 0; row < kMaxRow; row++) {
			for (int column = 0; column < kMaxColumn; column++) {
				m[row][column] = result.m[row][column];
			}
		}
	}


	/// user method ///

	static Matrix3x3 MakeScale(const Vector2f& scale);

	static Matrix3x3 MakeRotate(float theta);

	static Matrix3x3 MakeTranslate(const Vector2f& translate);

	static Matrix3x3 MakeAffin(const Vector2f& scale, float theta, const Vector2f& translate);

	static Matrix3x3 Inverse(const Matrix3x3& matrix);

	static Matrix3x3 Transpose(const Matrix3x3& matrix);

	// graphics //
	/* 正射影行列 */
	static Matrix3x3 MakeOrthographic(float left, float top, float right, float bottom);
	/* ビューポート変換 */
	static Matrix3x3 MakeViewport(float left, float top, float width, float height);

	static Vector2f Transform(const Vector2f& vector, const Matrix3x3& matrix);

	static void ScreenPrintf(int x, int y, const Matrix3x3& matrix);
};

