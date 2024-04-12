#pragma once

//*****************************************************************************************
// MT3用の描画クラス
// polygon3-4, polygonManager, light, 等を排除して
// zBufferやlightingの計算を排除したDrawerクラス
// 旧camera3Dが持っていたdraw関数を移行
//*****************************************************************************************

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
#include <Novice.h>
#include <Camera3D.h>
#include <MyVector2.h>

////////////////////////////////////////////////////////////////////////////////////////////
// PrimitiveDrawer class
////////////////////////////////////////////////////////////////////////////////////////////
class PrimitiveDrawer {
public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	//! @brief シングルトン設計
	static PrimitiveDrawer* GetInstance();

	//! @brief 描画するカメラの設定
	void SetCamera(Camera3D* camera) { camera_ = camera; }

	//! w -> ワールド座標
	//! l -> ローカル座標(worldMatrixが必須)

	
	void DrawLine(
		const Vector3f& w1, const Vector3f& w2, uint32_t color
	);


	void DrawTriangle(
		const Vector3f& l1, const Vector3f& l2, const Vector3f& l3,
		const Matrix4x4& worldMatrix,
		uint32_t color, FillMode fillMode = kFillModeSolid
	);

private:

	//=========================================================================================
	// private variables
	//=========================================================================================

	Camera3D* camera_ = nullptr;

	//=========================================================================================
	// private methods
	//=========================================================================================

	Vector2f ChangeScreenPos(const Vector3f& worldPos);
	Vector2f ChangeScreenPos(const Vector3f& loaclPos, const Matrix4x4& worldMatrix);

};
