#include <Novice.h>
#include <cmath>
#include <ImGui.h>
#include <memory>
#include <format>

// Lib //
#include "Environment.h"
#include "InputManager.h"
#include "GameTextureManager.h"

// Geometry //
#include "MyVector3.h"
#include "MyMatrix4x4.h"

// 3D //
#include "Camera3D.h"
#include "PrimitiveDrawer.h"
#include "Collider.h"

/***********************************
 * メイン関数 *
 ***********************************/
 // Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	using Input = InputManager;

	// camera
	std::unique_ptr<Camera3D> camera = std::make_unique<Camera3D>();

	// drawer
	auto drawer = PrimitiveDrawer::GetInstance();
	drawer->SetCamera(camera.get());

	OBB obb = {
		{ -1.0f, 0.0f, 0.0f },
		Matrix4x4::MakeIdentity(),
		{ 0.5f, 0.5f, 0.5f }
	};

	Segment segment = {
		.origin = {-0.8f, -0.3f, 0.0f},
		.diff   = {0.5f, 0.5f, 0.5f}
	};

	/***********************************
	 * ゲームループ *
	 ***********************************/
	 // ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		Input::Update();

		///
		/// ↓更新処理ここから
		///

		ImGui::Begin("Editor");
		camera->SetOnImGui();

		if (ImGui::TreeNode("OBB")) {
			obb.SetOnImGui();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Segment")) {
			segment.SetOnImGui();
			ImGui::TreePop();
		}

		bool isCollision = Collider::OBBToSegment(obb, segment);

		std::string text = std::format("isCollision: {}", isCollision);
		ImGui::Text(text.c_str());

		ImGui::End();

		uint32_t color = 0xFAFAFAFF;
		if (isCollision) {
			color = 0xFA0000FF;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		drawer->DrawGrid(
			{0.0f, 0.0f, 0.0f},
			4.0f, 10, 0x505050FF
		);

		drawer->DrawOBB(
			obb, color
		);

		drawer->DrawLine(
			segment.origin, segment.origin + segment.diff, 0xFAFAFAFF
		);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (Input::IsTriggerKeys(DIK_F4)) {
			break;
		}
	}

	camera.reset();

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}