#pragma once // version 2024/01/15 //

//-----------------------------------------------------------------------------------------
// Include
//-----------------------------------------------------------------------------------------
// 3D > Geometry ---
#include "MyVector2.h"
#include "MyVector3.h"
#include "MyMatrix4x4.h"

// 3D > ... ---
#include "Camera3D.h"
#include "Light.h"
#include "LightContainer.h"

// Lib > Polygon ---
#include "PolygonManager.h"

////////////////////////////////////////////////////////////////////////////////////////////
// Polygon4 class
////////////////////////////////////////////////////////////////////////////////////////////
class Polygon4 final {
public: // public method //

	Polygon4();
	~Polygon4();

	/* camera only */

	//! @brief 4角形ポリゴンを描画(PolygonManagerに追加)
	//! @brief 参考:blenderライト
	//! 
	//! @param[in] localPos    ローカル座標頂点
	//! @param[in] worldMatrix ワールド行列
	//! @param[in] color       デフォルトの色
	//! @param[in] camera      描画するカメラ
	//! @param[in] fillMode    FillMode 
	//! @param[in] BlendMode   BlendMode
	void DrawPolygon4Lambert(
		const Vector3f& localPosA, const Vector3f& localPosB, const Vector3f& localPosC, const Vector3f& localPosD,
		const Matrix4x4& worldMatrix, uint32_t color,
		const Camera3D& camera,
		FillMode fillMode = kFillModeSolid, BlendMode blendMode = kBlendModeNormal);

	//! @brief 4角形ポリゴンを描画(PolygonManagerに追加) texture描画
	//! @brief blenderライト
	//! 
	//! @param[in] localPos     ローカル座標
	//! @param[in] worldMatrix  ワールド行列
	//! @param[in] color        色
	//! @param[in] camera       描画するカメラ
	//! @param[in] kSubdivision 四角形ポリゴン分割数
	//! @param[in] texture      テクスチャハンドル
	//! @param[in] textureSize  テクスチャのサイズ
	//! @param[in] blendMode    BlendMode
	void DrawPolygon4Lambert(
		const Vector3f& localPosA, const Vector3f& localPosB, const Vector3f& localPosC, const Vector3f& localPosD,
		const Matrix4x4& worldMatrix, uint32_t color,
		const Camera3D& camera,
		uint32_t kSubdivision, int texture, const Vector2f& textureSize,
		BlendMode blendMode = kBlendModeNormal
	);

	/* --- Light& camera --- */

	//! @brief 4角形ポリゴンを描画(PolygonManagerに追加)
	//! @brief Lambert反射
	//! 
	//! @param[in] localPos    ローカル座標頂点
	//! @param[in] worldMatrix ワールド行列
	//! @param[in] color       デフォルトの色
	//! @param[in] camera      描画するカメラ
	//! @param[in] light       ライト
	//! @param[in] fillMode    FillMode 
	//! @param[in] BlendMode   BlendMode
	void DrawPolygon4Lambert(
		const Vector3f& localPosA, const Vector3f& localPosB, const Vector3f& localPosC, const Vector3f& localPosD,
		const Matrix4x4& worldMatrix, uint32_t color,
		const Camera3D& camera, const Light& light,
		FillMode fillMode = kFillModeSolid, BlendMode blendMode = kBlendModeNormal);

	//! @brief 4角形ポリゴンを描画(PolygonManagerに追加)
	//! @brief BlinnPhong反射
	//! 
	//! @param[in] localPos    ローカル座標頂点
	//! @param[in] worldMatrix ワールド行列
	//! @param[in] color       デフォルトの色
	//! @param[in] specPow     鏡面反射率
	//! @param[in] camera      描画するカメラ
	//! @param[in] light       ライト
	//! @param[in] fillMode    FillMode 
	//! @param[in] BlendMode   BlendMode
	void DrawPolygon4BlinnPhong(
		const Vector3f& localPosA, const Vector3f& localPosB, const Vector3f& localPosC, const Vector3f& localPosD,
		const Matrix4x4& worldMatrix, uint32_t color, float specPow,
		const Camera3D& camera, const Light& light,
		FillMode fillMode = kFillModeSolid, BlendMode blendMode = kBlendModeNormal);

	/* --- Light container --- */
	//! @brief 4角形ポリゴンを描画(PolygonManagerに追加)
	//! @brief Lambert反射
	//! 
	//! @param[in] localPos       ローカル座標頂点
	//! @param[in] worldMatrix    ワールド行列
	//! @param[in] color          デフォルトの色
	//! @param[in] camera         描画するカメラ
	//! @param[in] lightContainer ライトコンテナ
	//! @param[in] fillMode       FillMode 
	//! @param[in] BlendMode      BlendMode
	void DrawPolygon4Lambert(
		const Vector3f& localPosA, const Vector3f& localPosB, const Vector3f& localPosC, const Vector3f& localPosD,
		const Matrix4x4& worldMatrix, uint32_t color,
		const Camera3D& camera, const LightContainer& lightContainer,
		FillMode fillMode = kFillModeSolid, BlendMode blendMode = kBlendModeNormal);

	//! @brief 4角形ポリゴンを描画(PolygonManagerに追加)
	//! @brief BlinnPhong反射
	//! 
	//! @param[in] localPos       ローカル座標頂点
	//! @param[in] worldMatrix    ワールド行列
	//! @param[in] color          デフォルトの色
	//! @param[in] specPow        鏡面反射率
	//! @param[in] camera         描画するカメラ
	//! @param[in] lightContainer ライトコンテナ
	//! @param[in] fillMode       FillMode 
	//! @param[in] BlendMode      BlendMode
	void DrawPolygon4BlinnPhong(
		const Vector3f& localPosA, const Vector3f& localPosB, const Vector3f& localPosC, const Vector3f& localPosD,
		const Matrix4x4& worldMatrix, uint32_t color, float specPow,
		const Camera3D& camera, const LightContainer& lightContainer,
		FillMode fillMode = kFillModeSolid, BlendMode blendMode = kBlendModeNormal);

private: // menber object //

	PolygonManager* manager = PolygonManager::GetInstance();

	static const int kVertexNum_ = 4;
	enum VertexName {
		A, B, C, D
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

	// Fog
	float ndcDepth_;

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
		FillMode fillMode, BlendMode blendMode = kBlendModeNormal) const;

	void AddPolygonElement(
		uint32_t kSubdivision, int texture, const Vector2f& textureSize,
		BlendMode blendMode = kBlendModeNormal);
};

