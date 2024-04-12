#pragma once
#include <Novice.h>

#include <vector>
#include <algorithm>

#include <cassert>

#include <ImGui.h>

// Lib > Content //
#include "GameTextureManager.h"

// Lib > Geometry //
#include "MyVector3.h"
#include "MyVector2.h"

#include "PolygonElement.h"

enum SurfaceMode {
	kHiddenSurface,    //!< 影面消去あり
	kNotHiddenSurface, //!< 影面消去なし
};

////////////////////////////////////////////////////////////////////////////////////////////
// PolygonManager class
////////////////////////////////////////////////////////////////////////////////////////////
class PolygonManager final {
private:

	//=========================================================================================
	// private variables
	//=========================================================================================

	std::vector<SortPolygonElement> polygons_;

	enum VertexName {
		A, B, C, D,
	};

	//=========================================================================================
	// private method
	//=========================================================================================

	PolygonManager();
	~PolygonManager() = default;

	void SortPolygon();

	void DrawPolygonElement(const PolygonElement& element);

public: 

	//=========================================================================================
	// public method
	//=========================================================================================

	static PolygonManager* GetInstance();
	PolygonManager& operator=(const PolygonManager& obj) = delete;
	PolygonManager(const PolygonManager& obj) = delete;

	void AddPolygonElement(const SortPolygonElement& element);

	void DrawPolygon();

	void ClearPolygonStack();

	void ImGuiDebug();
};

