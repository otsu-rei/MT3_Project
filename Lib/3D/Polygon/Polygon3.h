#pragma once // version 2024/01/15 //

//-----------------------------------------------------------------------------------------
// Include
//-----------------------------------------------------------------------------------------
// Lib > Geometry ---
#include "MyVector3.h"
#include "MyMatrix4x4.h"

// 3D > ... ---
#include "Camera3D.h"
#include "Light.h"
#include "LightContainer.h"

// 3D > Polygon ---
#include "PolygonManager.h"

////////////////////////////////////////////////////////////////////////////////////////////
// Polygon3 class
////////////////////////////////////////////////////////////////////////////////////////////
class Polygon3 final {
public: // public method //

	Polygon3();
	~Polygon3();

	/* camera only */

	//! @brief 3角形ポリゴンを描画(PolygonManagerに追加)
	//! 
	//! @param[in] localPos    ローカル座標頂点
	//! @param[in] worldMatrix ワールド行列
	//! @param[in] color       デフォルトの色
	//! @param[in] camera      描画するカメラ
	//! @param[in] layer       レイヤー番号. (0はカメラソート)
	//! @param[in] fillMode    FillMode 
	//! @param[in] BlendMode   BlendMode
	void DrawPolygon3(
		const Vector3f& localPosA, const Vector3f& localPosB, const Vector3f& localPosC,
		const Matrix4x4& worldMatrix, uint32_t color,
		const Camera3D& camera, uint16_t layer = 0,
		FillMode fillMode = kFillModeSolid, BlendMode blendMode = kBlendModeNormal
	);

	//! @brief 3角形ポリゴンを描画(PolygonManagerに追加)
	//! @brief 参考:blenderライト
	//! 
	//! @param[in] localPos    ローカル座標頂点
	//! @param[in] worldMatrix ワールド行列
	//! @param[in] color       デフォルトの色
	//! @param[in] camera      描画するカメラ
	//! @param[in] layer       レイヤー番号. (0はカメラソート)
	//! @param[in] fillMode    FillMode 
	//! @param[in] BlendMode   BlendMode
	void DrawPolygon3Lambert(
		const Vector3f& localPosA, const Vector3f& localPosB, const Vector3f& localPosC,
		const Matrix4x4& worldMatrix, uint32_t color,
		const Camera3D& camera, uint16_t layer = 0,
		FillMode fillMode = kFillModeSolid, BlendMode blendMode = kBlendModeNormal
	);

	/* --- Light & camera --- */

	//! @brief 3角形ポリゴンを描画(PolygonManagerに追加)
	//! @brief Lambert反射
	//! 
	//! @param[in] localPos    ローカル座標頂点
	//! @param[in] worldMatrix ワールド行列
	//! @param[in] color       デフォルトの色
	//! @param[in] camera      描画するカメラ
	//! @param[in] light       ライト
	//! @param[in] layer       レイヤー番号. (0はカメラソート)
	//! @param[in] fillMode    FillMode 
	//! @param[in] BlendMode   BlendMode
	void DrawPolygon3Lambert(
		const Vector3f& localPosA, const Vector3f& localPosB, const Vector3f& localPosC,
		const Matrix4x4& worldMatrix, uint32_t color,
		const Camera3D& camera, const Light& light, uint16_t layer = 0,
		FillMode fillMode = kFillModeSolid, BlendMode blendMode = kBlendModeNormal
	);

	//! @brief 3角形ポリゴンを描画(PolygonManagerに追加)
	//! @brief BlinnPhong反射
	//! 
	//! @param[in] localPos    ローカル座標頂点
	//! @param[in] worldMatrix ワールド行列
	//! @param[in] color       デフォルトの色
	//! @param[in] specPow     鏡面反射率
	//! @param[in] camera      描画するカメラ
	//! @param[in] light       ライト
	//! @param[in] layer       レイヤー番号. (0はカメラソート)
	//! @param[in] fillMode    FillMode 
	//! @param[in] BlendMode   BlendMode
	void DrawPolygon3BlinnPhong(
		const Vector3f& localPosA, const Vector3f& localPosB, const Vector3f& localPosC,
		const Matrix4x4& worldMatrix, uint32_t color, float specPow,
		const Camera3D& camera, const Light& light, uint16_t layer = 0,
		FillMode fillMode = kFillModeSolid, BlendMode blendMode = kBlendModeNormal
	);


	/* --- Light container --- */

	//! @brief 3角形ポリゴンを描画(PolygonManagerに追加)
	//! @brief Lambert反射
	//! 
	//! @param[in] localPos       ローカル座標頂点
	//! @param[in] worldMatrix    ワールド行列
	//! @param[in] color          デフォルトの色
	//! @param[in] camera         描画するカメラ
	//! @param[in] lightContainer ライトコンテナ
	//! @param[in] layer          レイヤー番号. (0はカメラソート)
	//! @param[in] fillMode       FillMode 
	//! @param[in] BlendMode      BlendMode
	void DrawPolygon3Lambert(
		const Vector3f& localPosA, const Vector3f& localPosB, const Vector3f& localPosC,
		const Matrix4x4& worldMatrix, uint32_t color,
		const Camera3D& camera, const LightContainer& lightContainer, uint16_t layer = 0,
		FillMode fillMode = kFillModeSolid, BlendMode blendMode = kBlendModeNormal
	);

	//! @brief 3角形ポリゴンを描画(PolygonManagerに追加)
	//! @brief BlinnPhong反射
	//! 
	//! @param[in] localPos       ローカル座標頂点
	//! @param[in] worldMatrix    ワールド行列
	//! @param[in] color          デフォルトの色
	//! @param[in] specPow        鏡面反射率
	//! @param[in] camera         描画するカメラ
	//! @param[in] lightContainer ライトコンテナ
	//! @param[in] layer          レイヤー番号. (0はカメラソート)
	//! @param[in] fillMode       FillMode 
	//! @param[in] BlendMode      BlendMode
	void DrawPolygon3BlinnPhong(
		const Vector3f& localPosA, const Vector3f& localPosB, const Vector3f& localPosC,
		const Matrix4x4& worldMatrix, uint32_t color, float specPow,
		const Camera3D& camera, const LightContainer& lightContainer, uint16_t layer = 0,
		FillMode fillMode = kFillModeSolid, BlendMode blendMode = kBlendModeNormal
	);

private: // menber object //

	PolygonManager* manager = PolygonManager::GetInstance();

	static const int kVertexNum_ = 3;
	enum VertexName {
		A, B, C
	};

	Vector3f worldCenterPos_;
	float distance_;

	// screen
	Vector3f screenVerticesPos_[kVertexNum_];

	// direction
	Vector3f polygonDirection_;
	Vector3f cameraDirection_;
	Vector3f lightDirection_;

	// direction -> dot
	float polygon2cameraPosDot_;
	float polygon2cameraDirectionDot_;
	float polygon2lightDot_;


	// light
	float lightAmount_;

	// light container
	std::map<std::string, float> polygon2lightDotMap_;
	std::map<std::string, float> lightAmountMap_;
	std::map<std::string, Vector3f> lightDirectionMap_;

	// color
	uint32_t displayColor_;

private: // private method //

	void Finalize();

	/* --- polygon --- */
	void SetPolygonData(
		const Vector3f localPos[kVertexNum_], const Matrix4x4& worldMatrix);

	/* --- camera --- */
	void SetScreenPos(
		const Vector3f localPos[kVertexNum_], const Matrix4x4& worldMatrix, const Camera3D& camera);

	void SetCameraDot(
		const Camera3D& camera);

	/* --- light --- */
	// light
	void SetLightDot(
		const Light& light);

	void SetLightAmount(
		const Light& light);

	// light container
	void SetLightDot(
		const LightContainer& lightContainer);

	void SetLightAmount(
		const LightContainer& lightContainer);

	/* --- FixColor --- */
	// camera only
	void FixColorLambert(
		uint32_t color);

	// light
	void FixColorLambert(
		uint32_t color, const Light& light);

	void FixColorBlinnPhong(
		uint32_t color, float specPow, const Light& light);

	// light container
	void FixColorLambert(
		uint32_t color, const LightContainer& lightContainer);

	void FixColorBlinnPhong(
		uint32_t color, float specPow, const LightContainer& lightContainer);

	/* --- AddPolygon --- */
	void AddPolygonElement(
		uint16_t layer, FillMode fillMode = kFillModeSolid, BlendMode blendMode = kBlendModeNormal);
};

