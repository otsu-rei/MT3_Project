#include <Novice.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <ImGui.h>

// Lib //
#include "Environment.h"
#include "InputManager.h"
#include "GameTextureManager.h"

// Geometry //
#include "MyVector3.h"

/***********************************
 * メイン関数 *
 ***********************************/
 // Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	Vector3f v1 = { 1.0f, 3.0f, -5.0f };
	Vector3f v2 = { 4.0f, -1.0f, 2.0f };
	float k = 4.0f;

	const int kRowHeight = 20;

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

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		VectorNovice::ScreenPrintf(0, 0,  v1 + v2, ": Add");
		VectorNovice::ScreenPrintf(0, kRowHeight, v1 - v2, ": Subtract");
		VectorNovice::ScreenPrintf(0, kRowHeight * 2, v1 * k, ": Multiply");
		Novice::ScreenPrintf(0, kRowHeight * 3, "%.02f : Dot", Vector::Dot(v1, v2));
		Novice::ScreenPrintf(0, kRowHeight * 4, "%.02f : Length", Vector::Length(v1));
		VectorNovice::ScreenPrintf(0, kRowHeight * 5, Vector::Normalize(v2), ": Normalize");

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

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}