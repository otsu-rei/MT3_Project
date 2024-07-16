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

	Pendulum pendulum = {};
	pendulum.anchor              = { 0.0f, 1.0f, 0.0f };
	pendulum.length              = 0.8f;
	pendulum.angle               = 0.7f;
	pendulum.angularVelocity     = 0.0f;
	pendulum.angularAcceleration = 0.0f;

	Vector3f ballPosition = { 0.0f };

	ballPosition.x = pendulum.anchor.x + std::sin(pendulum.angle) * pendulum.length;
	ballPosition.y = pendulum.anchor.y - std::cos(pendulum.angle) * pendulum.length;
	ballPosition.z = pendulum.anchor.z;

	bool isUpdate = false;

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

		ImGui::Checkbox("isUpdate", &isUpdate);

		ImGui::End();

		if (isUpdate) {

			pendulum.angularAcceleration
				= -(kGrabity / pendulum.length) * std::sin(pendulum.angle);

			pendulum.angularVelocity += pendulum.angularAcceleration * deltaTime;
			pendulum.angle += pendulum.angularVelocity * deltaTime;

			ballPosition.x = pendulum.anchor.x + std::sin(pendulum.angle) * pendulum.length;
			ballPosition.y = pendulum.anchor.y - std::cos(pendulum.angle) * pendulum.length;
			ballPosition.z = pendulum.anchor.z;

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

		drawer->DrawLine(
			pendulum.anchor, ballPosition, 0xFAFAFAFF
		);

		drawer->DrawSphere(
			ballPosition, 0.04f, 16, 0xFAFAFAFF
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