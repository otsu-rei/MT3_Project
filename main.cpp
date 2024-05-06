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

	// main
	Sphere sphere[2];

	for (int i = 0; i < 2; ++i) {
		sphere[i].radius = 0.5f;
		sphere[i].center = { i * 1.0f, i * 1.0f, i * 1.0f };
	}

	uint32_t color = 0xFAFAFAFF;

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

		ImGui::Separator();

		for (int i = 0; i < 2; ++i) {
			std::string label = std::format("sphere[{}]", i);

			if (ImGui::TreeNode(label.c_str())) {
				ImGui::DragFloat3("center", &sphere[i].center.x, 0.01f);
				ImGui::DragFloat("radius",  &sphere[i].radius,   0.01f);

				ImGui::TreePop();
			}
		}

		color = 0xFAFAFAFF; //!< default color

		if (Collider::SphereTo(sphere[0], sphere[1])) {
			color = 0xFA0000FF;
		}

		ImGui::End();

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

		for (int i = 0; i < 2; ++i) {
			drawer->DrawSphere(
				sphere[i].center, sphere[i].radius,
				16, color
			);
		}

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