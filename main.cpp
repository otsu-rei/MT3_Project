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

	Plane plane = {
		{0.0f, 1.0f, 0.0f},
		1.0f
	};

	Sphere sphere = {
		{0.0f, 0.0f, 0.0f},
		1.0f
	};

	uint32_t color;

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

		if (ImGui::TreeNode("plane")) {
			ImGui::DragFloat3("normal", &plane.normal.x, 0.02f);
			plane.normal = Vector::Normalize(plane.normal);

			ImGui::DragFloat("distance", &plane.distance, 0.02f);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("sphere")) {
			ImGui::DragFloat3("center", &sphere.center.x, 0.02f);
			ImGui::DragFloat("radius",  &sphere.radius, 0.02f);
			ImGui::TreePop();
		}

		ImGui::End();

		color = 0xFAFAFAFF;
		if (Collider::PlaneToSphere(plane, sphere)) {
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

		drawer->DrawPlane(
			plane, 0xFAFAFAFF
		);

		drawer->DrawSphere(
			sphere.center, sphere.radius, 16, color
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