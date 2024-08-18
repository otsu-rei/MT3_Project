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

	bool isUpdate = false;

	Ball ball = {};
	ball.position     = { 0.8f, 1.2f, 0.3f };
	ball.acceleration = { 0.0f, -kGrabity, 0.0f };
	ball.mass         = 2.0f;
	ball.radius       = 0.05f;
	ball.color        = 0xFAFAFAFF;

	Plane plane = {};
	plane.normal   = Vector::Normalize({-0.2f, 1.2f, -0.3f});
	plane.distance = 0.0f;

	const float e = 0.8f;

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

			ball.velocity += ball.acceleration * deltaTime;
			ball.position += ball.velocity * deltaTime;

			ball.color = 0xFAFAFAFF;

			// capsuleの生成
			Capsule capsule = {};
			capsule.radius         = ball.radius;
			capsule.segment.origin = ball.position;
			capsule.segment.diff   = ball.velocity * deltaTime;

			if (Collider::PlaneToCapsule(plane, capsule, &ball.position)) {

				ball.color = 0xFA0000FF;

				Vector3f reflected = Vector::Reflect(ball.velocity, plane.normal);
				Vector3f projectToNormal = Project(reflected, plane.normal);
				Vector3f movingDirction = reflected - projectToNormal;
				ball.velocity = projectToNormal * e + movingDirction;
			}

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