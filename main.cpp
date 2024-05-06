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
	Segment segment = { {-2.0f, -1.0f, 0.0f}, {3.0f, 2.0f, 2.0f} };
	Vector3f point = { -1.5f, 0.6f, 0.6f };

	Vector3f project; 
	Vector3f closestPoint;

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

		if (ImGui::CollapsingHeader("Segment")) {
			ImGui::DragFloat3("origin", &segment.origin.x, 0.01f);
			ImGui::DragFloat3("diff",   &segment.diff.x,   0.01f);
		}

		if (ImGui::CollapsingHeader("point")) {
			ImGui::DragFloat3("position", &point.x, 0.01f);
		}

		project = Project(point - segment.origin, segment.diff);
		closestPoint = ClosestPoint(point, segment);

		ImGui::Spacing();

		ImGui::Text(
			"[project] x: %.3f, y: %.3f, z: %.3f",
			project.x, project.y, project.z
		);

		ImGui::Text(
			"[closestPoint] x: %.3f, y: %.3f, z: %.3f",
			closestPoint.x, closestPoint.y, closestPoint.z
		);

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

		drawer->DrawLine(
			segment.origin,
			segment.origin + segment.diff,
			0xFAFAFAFF
		);

		drawer->DrawSphere(
			point, 0.01f, 10,
			0xFA0000FF
		);

		drawer->DrawSphere(
			closestPoint, 0.01f, 10,
			0x0F0F0FFF
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