#include "MyMatrix3x3.h"

Matrix3x3 Matrix3x3::MakeScale(const Vector2f& scale) {
	return {
		scale.x, 0, 0,
		0, scale.y, 0,
		0, 0, 1,
	};
}

Matrix3x3 Matrix3x3::MakeRotate(float theta) {
	return {
		std::cos(theta), std::sin(theta), 0,
		-std::sin(theta), std::cos(theta), 0,
		0, 0, 1,
	};
}

Matrix3x3 Matrix3x3::MakeTranslate(const Vector2f& translate) {
	return {
		1, 0, 0,
		0, 1, 0,
		translate.x, translate.y, 1,
	};
}

Matrix3x3 Matrix3x3::MakeAffin(const Vector2f& scale, float theta, const Vector2f& translate) {
	return {
		scale.x * cosf(theta), scale.x * sinf(theta), 0,
		scale.y * -sinf(theta), scale.y * cosf(theta), 0,
		translate.x, translate.y, 1,
	};
}

Matrix3x3 Matrix3x3::Inverse(const Matrix3x3& matrix) {
	Matrix3x3 result{
		matrix.m[1][1] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][1],    // [0][0]
		-(matrix.m[0][1] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[2][1]), // [0][1]
		matrix.m[0][1] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][1],    // [0][2]

		-(matrix.m[1][0] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][0]), // [1][0]
		matrix.m[0][0] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[2][0],    // [1][1]
		-(matrix.m[0][0] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][0]), // [1][2]

		matrix.m[1][0] * matrix.m[2][1] - matrix.m[1][1] * matrix.m[2][0],    // [2][0]
		-(matrix.m[0][0] * matrix.m[2][1] - matrix.m[0][1] * matrix.m[2][0]), // [2][1]
		matrix.m[0][0] * matrix.m[1][1] - matrix.m[0][1] * matrix.m[1][0],    // [2][2]
	};

	float denominator = (matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2])
		+ (matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0])
		+ (matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1])
		- (matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0])
		- (matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2])
		- (matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1]);

	assert(denominator != 0);

	for (int row = 0; row < 3; row++) {
		for (int column = 0; column < 3; column++) {
			result.m[row][column] *= 1.0f / denominator;
		}
	}

	return result;
}

Matrix3x3 Matrix3x3::Transpose(const Matrix3x3& matrix) {
	return {
		matrix.m[0][0], matrix.m[1][0], matrix.m[2][0],
		matrix.m[0][1], matrix.m[1][1], matrix.m[2][1],
		matrix.m[0][2], matrix.m[1][2], matrix.m[2][2],
	};
}

// graphics //
Matrix3x3 Matrix3x3::MakeOrthographic(float left, float top, float right, float bottom) {
	return {
		2.0f / (right - left), 0.0f, 0.0f,
		0.0f, 2.0f / (top - bottom), 0.0f,
		(left + right) / (left - right), (top + bottom) / (bottom - top), 1.0f
	};
}

Matrix3x3 Matrix3x3::MakeViewport(float left, float top, float width, float height) {
	return {
		width / 2.0f, 0.0f, 0.0f,
		0.0f, -(height) / 2.0f, 0.0f,
		left + (width / 2.0f), top + (height / 2.0f), 1.0f
	};
}

Vector2f Matrix3x3::Transform(const Vector2f& vector, const Matrix3x3& matrix) {
	Vector2f result;
	float w;

	/// Update ///
	result.x = (vector.x * matrix.m[0][0]) + (vector.y * matrix.m[1][0]) + (1.0f * matrix.m[2][0]);
	result.y = (vector.x * matrix.m[0][1]) + (vector.y * matrix.m[1][1]) + (1.0f * matrix.m[2][1]);
	w = (vector.x * matrix.m[0][2]) + (vector.y * matrix.m[1][2]) + (1.0f * matrix.m[2][2]);

	assert(w != 0.0f);

	result.x /= w;
	result.y /= w;

	return result;
}

void Matrix3x3::ScreenPrintf(int x, int y, const Matrix3x3& matrix) {
	for (int row = 0; row < 3; row++) {
		for (int column = 0; column < 3; column++) {
			Novice::ScreenPrintf(
				x + (column * kColomWidth), y + (row * kRowHeight), "%.02f", matrix.m[row][column]
			);
		}
	}
}
