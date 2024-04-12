#pragma once
#include <Novice.h>

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
// Lib > Geometry ---
#include "MyVector3.h"
#include "MyVector2.h"


static const int kMaxPolygonVertexNum = 4;

////////////////////////////////////////////////////////////////////////////////////////////
// PolygonType enum
////////////////////////////////////////////////////////////////////////////////////////////
enum PolygonType {
	kPolygonTriangle,
	kPolygonQuad,
	kPolygonQuadTexture,

	SelectNone,
};

////////////////////////////////////////////////////////////////////////////////////////////
// PolygonElement class
////////////////////////////////////////////////////////////////////////////////////////////
class PolygonElement {
public:

	PolygonType polygonType = SelectNone; //!< ポリゴンタイプ

	Vector3f screenPos[kMaxPolygonVertexNum]; //!< スクリーン頂点
	uint32_t color;                           //!< カラー

	FillMode fillMode;   //!< Novice FillMode
	BlendMode blendMode; //!< Novice BlendMode

	int texture;          //!< テクスチャグラフハンドル
	Vector2f textureSize; //!< テクスチャのサイズ

	uint32_t subdivision; //!< テクスチャの分割数
};

class SortPolygonElement
	: public PolygonElement {
public:

	uint16_t layer;
	float distance;

};