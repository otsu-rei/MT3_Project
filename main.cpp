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
	ball.position = { 0.8f, 0.2f, 0.0f };
	ball.mass     = 2.0f;
	ball.radius   = 0.05f;
	ball.color    = 0x0000FAFF;

	Spring spring = {};
	spring.anchor             = { 0.0f, 1.0f, 0.0f };
	spring.natureLength       = 0.7f;
	spring.stiffness          = 1.0f;
	spring.dampingCoefficient = 2.0f;

	const Vector3f kGravity = { 0.0f, -9.8f, 0.0f };

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

		if (ImGui::TreeNode("spring")) {
			spring.SetImGui();
			ImGui::TreePop();
		}

		ImGui::Checkbox("isUpdate", &isUpdate);

		ImGui::End();

		if (isUpdate) {

			// spring 
			Vector3f diff = ball.position - spring.anchor;
			float length = Vector::Length(diff);

			if (length != 0.0f) {
				Vector3f direciton = Vector::Normalize(diff);
				Vector3f restPosition = spring.anchor + direciton * spring.natureLength;
				Vector3f displacement = length * (ball.position - restPosition);
				Vector3f restoringForce = -spring.stiffness * displacement;
				Vector3f dampingForce = -spring.dampingCoefficient * ball.velocity;
				Vector3f force = restoringForce + dampingForce;

				ball.acceleration = (force + kGravity) / ball.mass;
			}

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
			ball.position, ball.radius, 16, ball.color
		);

		drawer->DrawLine(
			ball.position, spring.anchor, 0xFAFAFAFF
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