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
	
	Transform shoulder = {
		unitVector,
		{ 0.0f, 0.0f, -6.8f },
		{ 0.2f, 1.0f, 0.0f }
	};

	Transform elbow = {
		unitVector,
		{ 0.0f, 0.0f, -1.4f },
		{ 0.4f, 0.0f, 0.0f }
	};

	Transform hand = {
		unitVector,
		{ 0.0f, 0.0f, 0.0f },
		{ 0.3f, 0.0f, 0.0f }
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

		if (ImGui::TreeNode("shoulder")) {

			shoulder.DragTransform();

			if (ImGui::TreeNode("elbow")) {

				elbow.DragTransform();

				if (ImGui::TreeNode("hand")) {

					hand.DragTransform();

					ImGui::TreePop();
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		ImGui::End();

		Matrix4x4 shoulderWorldMat = shoulder.CreateMatrix();
		Matrix4x4 elbowWorldMat    = elbow.CreateMatrix() * shoulderWorldMat;
		Matrix4x4 handWorldMat     = hand.CreateMatrix() * elbowWorldMat;

		Vector3f worldPositions[3];
		worldPositions[0] = Matrix::Transform(origin, shoulderWorldMat);
		worldPositions[1] = Matrix::Transform(origin, elbowWorldMat);
		worldPositions[2] = Matrix::Transform(origin, handWorldMat);

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

		drawer->DrawSphere(
			worldPositions[0], 0.02f, 16, 0xFA0000FF
		);

		drawer->DrawSphere(
			worldPositions[1], 0.02f, 16, 0x00FA00FF
		);

		drawer->DrawSphere(
			worldPositions[2], 0.02f, 16, 0x0000FAFF
		);

		for (int i = 0; i < 2; ++i) {
			drawer->DrawLine(
				worldPositions[i], worldPositions[i + 1], 0xFAFAFAFF
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