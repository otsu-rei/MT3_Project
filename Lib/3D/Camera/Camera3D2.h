#pragma once

//-----------------------------------------------------------------------------------------
// Include
//-----------------------------------------------------------------------------------------
#include <Novice.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <ImGui.h>

// Lib ---
#include "Environment.h"
#include "InputManager.h"
#include "Json.h"

// 3D > Geometry ---
#include "MyVector3.h"
#include "MyMatrix4x4.h"

enum AxisType {
	IN_COLOR,
	NOT_COLOR,
};

enum LoadType {
	LOAD_DATA,    //!< scale, rotate, translate を読み込み
	LOAD_BLENDER, //!< controlPoint, lon, lat, distance を読み込み
};

////////////////////////////////////////////////////////////////////////////////////////////
// Camera3D class
////////////////////////////////////////////////////////////////////////////////////////////
class Camera3D final {
public: // menber method //

	// constructor & destructor
	Camera3D();
	~Camera3D();

	// default method
	void Init();

public: // user method //

	/* -----------------------------------------------------------------------------------------
	 グラフィックスパイプライン
	----------------------------------------------------------------------------------------- */
	static Vector3f CheckGraphicPipeline(
		const Vector3f& localPos, const Matrix4x4& worldMatrix, const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix, const Matrix4x4& viewportMatrix);

	Vector3f CheckGraphicPipeline(
		const Vector3f& localPos, const Matrix4x4& worldMatrix);

	/* -----------------------------------------------------------------------------------------
	 カメラ情報 設定
	----------------------------------------------------------------------------------------- */
	void SetCameraImGui(
		const char* windowName, const char* cameraName);

	void SetCamera(
		const Vector3f& scale, const Vector3f& rotate, const Vector3f& translate);

	void SetCamera(
		const Vector3f& translate);

	void SetProjection(
		float fovY, float aspectRatio, float nearClip, float farClip);

	void SetViewport(
		float left, float top, float width, float height, float minDepth, float maxDepth);

	/* -----------------------------------------------------------------------------------------
	 カメラ視点をアップデート
	----------------------------------------------------------------------------------------- */
	void UpdateCamera(); // reference blender

	/* -----------------------------------------------------------------------------------------
	 カメラ視点の設定
	----------------------------------------------------------------------------------------- */
	void SetCameraPoint(
		const Vector3f& point, float lon, float lat, float distance);

	/* -----------------------------------------------------------------------------------------
	 標準描画関数
	----------------------------------------------------------------------------------------- */
	void DrawGrid(
		float gridHalfLength, uint32_t kSubdivision, uint32_t color, AxisType axisType = IN_COLOR) const;

	void DrawSphere(
		const Vector3f& center, float radius, uint32_t kSubdivision, uint32_t color) const;

	void DrawCoordinate(
		const Vector3f& center, float length) const;

	void DrawCoordinate(
		const Matrix4x4& worldMatrix, float length) const;

	// 他カメラの描画
	void DrawOtherCamera( // TODO:
		const Camera3D& other);


	/* -----------------------------------------------------------------------------------------
	 カメラ情報 json
	----------------------------------------------------------------------------------------- */
	//! @brief カメラ情報をjsonに書き込み
	//! 
	//! @param[in] cameraName カメラの名前. 保存名:[directory] + [cameraName].json;
	//! 
	//! @retval true  書き込みに成功
	//! @retval false 書き込みに失敗
	bool SaveCameraData(const std::string& cameraName);

	//! @brief カメラ情報をjsonから読み込み
	//! 
	//! @param[in] cameraName カメラの名前. 保存名:[directory] + [cameraName].json;
	//! @param[in] type       ロードタイプ
	//! 
	//! @retval true  読み込みに成功
	//! @retval false 読み込みに失敗
	bool LoadCameraData(const std::string& cameraName, LoadType type);

	bool SaveProjection(const std::string& cameraName);

	/* -----------------------------------------------------------------------------------------
	 カメラ情報デバック
	----------------------------------------------------------------------------------------- */
	void DebugCamera(
		const char* windowName = "WindowName", const char* cameraName = "Camera");

private: // menber object //

	// camera data
	struct Camera {
		Vector3f scale;
		Vector3f rotate;
		Vector3f translate;
	};

	Camera camera_;

	Matrix4x4 viewMatrix_;
	Matrix4x4 projectionMatrix_;
	Matrix4x4 viewportMatrix_;

	// demo
	Vector3f direction_;

	// camera control
	Vector3f controlPoint_;
	float lon_;
	float lat_;
	float distance_;

	// json
	const std::string directory_ = JsonClass::jsonFile + "camera/";

private: // detail method //

	// function
	void UpdateCameraData();

public: // Getter //

	Matrix4x4 GetViewProjectionMatrix() const { return viewMatrix_ * projectionMatrix_; }
	Matrix4x4 GetViewportMatrix() const { return viewportMatrix_; }
	Vector3f GetCameraTranslate() const { return camera_.translate; }
	Vector3f GetCameraRotate() const { return camera_.rotate; }

	Camera GetCamera() const { return camera_; }

	Vector3f GetCameraDirection() const { return direction_; }
};

