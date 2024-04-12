#include <Novice.h>
#include <cmath>
#include <ImGui.h>
#include <memory>

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

/***********************************
 * メイン関数 *
 ***********************************/
 // Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// camera
	std::unique_ptr<Camera3D> camera = std::make_unique<Camera3D>();
	Vector3f rotate = origin;

	// drawer
	auto drawer = PrimitiveDrawer::GetInstance();
	drawer->SetCamera(camera.get());

	// cross
	Vector3f v1 = { 1.2f, -3.9f, 2.5f };
	Vector3f v2 = { 2.8f, 0.4f, -1.3f };

	/***********************************
	 * ゲームループ *
	 ***********************************/
	 // ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		InputManager::Update();

		///
		/// ↓更新処理ここから
		///

		ImGui::Begin("Editor");
		camera->SetOnImGui();
		ImGui::End();

		rotate.y += 0.02f;

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		drawer->DrawTriangle(
			{1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
			Matrix::MakeAffine(unitVector, rotate, origin),
			0xFF0000FF
		);

		VectorNovice::ScreenPrintf(0, 0, Vector::Cross(v1, v2), "cross");

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (InputManager::IsTriggerKeys(DIK_F4)) {
			break;
		}
	}

	camera.reset();

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}