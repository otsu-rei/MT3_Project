#include "MyVector3.h"

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
#include <cmath>
#include <Novice.h>

void VectorNovice::ScreenPrintf(int x, int y, const Vector3f& v, const char* label) {
	Novice::ScreenPrintf(x                   , y, "%.02f", v.x);
	Novice::ScreenPrintf(x + kColumnWidth * 1, y, "%.02f", v.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", v.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s"   , label);
}
