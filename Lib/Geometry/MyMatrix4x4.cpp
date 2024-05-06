#include "MyMatrix4x4.h"
#include <Novice.h>

////////////////////////////////////////////////////////////////////////////////////////////
// MatrixNovice namespace methods
////////////////////////////////////////////////////////////////////////////////////////////

void MatrixNovice::ScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			Novice::ScreenPrintf(
				x + col * kColumnWidth, y + row * kRowHeight,
				"%6.02f", matrix.m[row][col]
			);
		}
	}

	Novice::ScreenPrintf(x + 4 * kColumnWidth, y, label);
}
