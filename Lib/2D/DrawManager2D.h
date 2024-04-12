#pragma once

//-----------------------------------------------------------------------------------------
// Include
//-----------------------------------------------------------------------------------------
#include <Novice.h>
#include <vector>
#include <algorithm>

#include "DrawElement.h"

////////////////////////////////////////////////////////////////////////////////////////////
// DrawManager2D class
////////////////////////////////////////////////////////////////////////////////////////////
class DrawManager2D final {
public:

	//=========================================================================================
	// public methods
	//=========================================================================================
	static DrawManager2D* GetInstance();
	DrawManager2D& operator=(const DrawManager2D& obj) = delete;
	DrawManager2D(const DrawManager2D& obj) = delete;

	void AddElement(LayerDrawElement element);

	void Draw();

	void ClearElementStack();

private:

	//=========================================================================================
	// private variables
	//=========================================================================================
	std::vector<LayerDrawElement> elements_;

	static const uint32_t kMaxLayer_ = UINT32_MAX;
	uint32_t elementNum_;

	//=========================================================================================
	// private methods
	//=========================================================================================
	DrawManager2D();
	~DrawManager2D() = default;

	void Init();

	void DrawElements(const DrawElement& element);
};