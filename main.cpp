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

	ConicalPendulum pendulum = {};
	pendulum.anchor = { 0.0f, 1.0f, 0.0f };
	pendulum.length = 0.8f;
	pendulum.halfApexAngle = 0.7f;
	pendulum.angle = 0.0f;
	pendulum.angularVelocity = 0.0f;

	float radius = std::sin(pendulum.halfApexAngle) * pendulum.length;
	float height = std::cos(pendulum.halfApexAngle) * pendulum.length;

	Vector3f position = {
		pendulum.anchor.x + std::cos(pendulum.angle) * radius,
		pendulum.anchor.y - height,
		pendulum.anchor.z - std::sin(pendulum.angle) * radius
	};

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

			pendulum.angularVelocity = std::sqrt(kGrabity / (pendulum.length * std::cos(pendulum.halfApexAngle)));
			pendulum.angle += pendulum.angularVelocity * deltaTime;

			radius = std::sin(pendulum.halfApexAngle) * pendulum.length;
			height = std::cos(pendulum.halfApexAngle) * pendulum.length;

			position = {
				pendulum.anchor.x + std::cos(pendulum.angle) * radius,
				pendulum.anchor.y - height,
				pendulum.anchor.z - std::sin(pendulum.angle) * radius
			};

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
			pendulum.anchor, position, 0xFAFAFAFF
		);

		drawer->DrawSphere(
			position, 0.04f, 16, 0xFAFAFAFF
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