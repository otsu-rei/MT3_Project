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

#include <Random.h>

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

	Vector3f u = {}, v = {};

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

		ImGui::End();

		if (direciton.x == 0.0f && direciton.y == 0.0f) {
			u = { 1.0f, 0.0f, 0.0f };

		} else {
			u = Vector::Normalize({ -direciton.y, direciton.x, 0.0f });
		}

		v = Vector::Normalize(Vector::Cross(direciton, u));
		
		float theta = Random::Generate(-pi_v / 2.0f, pi_v / 2.0f);
		Vector3f spDir = {

		};

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
			position, position + direciton, 0xFA0000FF
		);

		drawer->DrawLine(
			position, position + u, 0x00FA00FF
		);

		drawer->DrawLine(
			position, position + v, 0x0000FAFF
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