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

	Vector3f a = { 0.2f, 1.0f, 0.0f };
	Vector3f b = { 2.4f, 3.1f, 1.2f };
	Vector3f rotate = { 0.4f, 1.43f, -0.8f };

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

		if (ImGui::TreeNode("Calculation")) {
			ImGui::DragFloat3("a", &a.x, 0.01f);
			ImGui::DragFloat3("b", &b.x, 0.01f);
			ImGui::DragFloat3("rotate", &rotate.x, 0.01f);
			ImGui::Spacing();
			
			ImGui::Text("result");
			ImGui::Separator();

			Vector3f add      = a + b;
			Vector3f subtract = a - b;
			Vector3f mul      = a * 2.4f;

			Matrix4x4 mat = Matrix::MakeRotate(rotate);

			ImGui::Text("a + b:    x = %f, y = %f, z = %f", add.x, add.y, add.z);
			ImGui::Text("a - b:    x = %f, y = %f, z = %f", subtract.x, subtract.y, subtract.z);
			ImGui::Text("a * 2.4f: x = %f, y = %f, z = %f", mul.x, mul.y, mul.z);

			ImGui::Text(
				"matrix: \n %f, %f, %f, %f \n %f, %f, %f, %f \n %f, %f, %f, %f \n %f, %f, %f, %f",
				mat.m[0][0], mat.m[0][1], mat.m[0][2], mat.m[0][3],
				mat.m[1][0], mat.m[1][1], mat.m[1][2], mat.m[1][3],
				mat.m[2][0], mat.m[2][1], mat.m[2][2], mat.m[2][3],
				mat.m[3][0], mat.m[3][1], mat.m[3][2], mat.m[3][3]
			);

			ImGui::TreePop();
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