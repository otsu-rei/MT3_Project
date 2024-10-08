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
#include "Physics.h"

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

	Vector3f position = { 0.0f, 0.0f, 0.0f };
	Vector3f direciton = { 1.0f, 0.0f, 0.0f };

	float angle = pi_v / 2.0f;

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

		ImGui::DragFloat3("direction", &direciton.x, 0.01f);
		direciton = Vector::Normalize(direciton);

		ImGui::SliderAngle("angle", &angle);

		ImGui::End();
		


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		drawer->DrawGrid(
			{ 0.0f, 0.0f, 0.0f },
			4.0f, 10, 0x505050FF
		);

		drawer->DrawLine(
			position, position + direciton, 0x00FA00FF
		);

		Vector3f newDir = Matrix::Transform(direciton, Matrix::MakeRotateEuler({angle, angle, 0.0f}));

		drawer->DrawLine(
			position, position + newDir, 0x00FAFAFF
		);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// F4キーが押されたらループを抜ける
		if (Input::IsTriggerKeys(DIK_F4)) {
			break;
		}
	}

	camera.reset();

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}