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

	Ball ball = {};
	ball.position = { 0.0f };
	ball.mass     = 2.0f;
	ball.radius   = 0.05f;
	ball.color    = 0xFAFAFAFF;

	Vector3f center = { 0.0f };
	float radius = 0.8f; //!< 半径
	float omega = pi_v;        //!< 角速度
	float angle = 0.0f;

	// 位置の初期化
	ball.position.x = center.x + std::cos(angle) * radius;
	ball.position.y = center.y + std::sin(angle) * radius;
	ball.position.z = center.z;

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

		if (ImGui::TreeNode("ball")) {
			ball.SetImGui();
			ImGui::TreePop();
		}

		ImGui::Checkbox("isUpdate", &isUpdate);

		ImGui::End();

		if (isUpdate) {

			angle += omega * deltaTime;

			ball.velocity.x = -radius * omega * std::sin(angle);
			ball.velocity.y = radius * omega * std::cos(angle);
			
			ball.acceleration.x = -(omega * omega) * radius * std::cos(angle);
			ball.acceleration.y = -(omega * omega) * radius * std::sin(angle);

			ball.velocity += ball.acceleration * deltaTime;
			ball.position += ball.velocity * deltaTime;

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

		drawer->DrawSphere(
			center, 0.02f, 16, 0x00FA00FF
		);

		drawer->DrawSphere(
			ball.position, ball.radius, 16, ball.color
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