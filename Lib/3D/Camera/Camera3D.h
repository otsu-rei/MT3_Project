#pragma once

//*****************************************************************************************
// MT3課題用のカメラクラス
// drawGridやdrawLineを削除
//*****************************************************************************************

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
#include "MyVector3.h"
#include "MyMatrix4x4.h"

////////////////////////////////////////////////////////////////////////////////////////////
// Transform structure
////////////////////////////////////////////////////////////////////////////////////////////
struct Transform {
	Vector3f scale     = unitVector;
	Vector3f rotate    = origin;
	Vector3f translate = origin;
};

////////////////////////////////////////////////////////////////////////////////////////////
// Camera3D class
////////////////////////////////////////////////////////////////////////////////////////////
class Camera3D {
public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	//! @brief コンストラクタ
	Camera3D() { Init(); }

	//! @brief 初期化処理
	void Init(); //!< MT3でのセットアップ

	//! @brief transform_を元にviewMatrixの生成
	void UpdateMatrix();

	//! @brief projectionを設定してMatrixの生成
	//! 
	//! @param[in] ...
	void SetProjection(float fovY, float aspectRatio, float nearClip, float farClip);

	//! @brief viewportを設定してMatrixの生成
	//! 
	//! @param[in] ...
	void SetViewport(float left, float top, float width, float height, float minDepth, float maxDepth);

	//! @brief viewMatrix * projMatrix の取得
	Matrix4x4 GetViewProjMatrix() const { return viewMatrix_ * projectionMatrix_; }

	//! @brief viewprotMatrix の取得
	const Matrix4x4& GetViewportMatrix() const { return viewportMatrix_; }

	//! @brief カメラTransform の取得
	const Transform& GetTransform() const { return transform_; }

	void SetOnImGui();

	//=========================================================================================
	// public variables
	//=========================================================================================

	Transform transform_;

private:

	////////////////////////////////////////////////////////////////////////////////////////////
	// Control structure
	////////////////////////////////////////////////////////////////////////////////////////////
	struct Control {
		Vector3f center = origin;
		float lon = 0.0f, lat = 0.0f;
		float distance = 10.0f;
	};

	//=========================================================================================
	// private variables
	//=========================================================================================

	Matrix4x4 viewMatrix_;
	Matrix4x4 projectionMatrix_;
	Matrix4x4 viewportMatrix_;

	// debug追加機能として
	Control control_;

};