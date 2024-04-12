#pragma once

//-----------------------------------------------------------------------------------------
// Include
//-----------------------------------------------------------------------------------------
#include <Novice.h>
#include <cstdint>

#include "MyVector2.h"

enum DrawType {
	kTriangle,
	kBox,
	kEllipse,
	kQuad,
	kSprite,
	kSpriteRect,

	Select_None
};

class DrawElement {
public:

	DrawType type = Select_None; //!< DrawType

	/* shapes data */
	Vector2f pos[4]; //!< スクリーン座標. (Quad用に配列[4]. Triangleは[0][1][2], その他は[0]参照)
	Vector2f size;   //!< スクリーンサイズ.

	float angle; //!< アングル

	uint32_t color; //!< カラー

	/* mode data */
	FillMode fillMode;   //!< FillMode
	BlendMode blendMode; //!< BlendMode

	/* texture data */
	int texture; //!< テクスチャハンドル
	
	Vector2i textureFullSize; //!< テクスチャのサイズ

	Vector2i textureRectPos;  //!< テクスチャ切り取り座標
	Vector2i textureRectSize; //!< テクスチャ切り取りサイズ
};

class LayerDrawElement
	: public DrawElement {
public:

	uint32_t layer; //!< レイヤー数

};

