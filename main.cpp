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
#include "MyMatrix4x4.h"

/***********************************
 * メイン関数 *
 ***********************************/
 // Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	const int kMatrixHeight = MatrixNovice::kRowHeight * 5;

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

		MatrixNovice::ScreenPrintf(
			0, 0,
			Matrix::MakeOrthographic(-160.0f, 160.0f, 200.0f, 300.0f, 0.0f, 1000.0f),
			"orthographic"
		);

		MatrixNovice::ScreenPrintf(
			0, kMatrixHeight,
			Matrix::MakePerspectiveFov(0.63f, 1.33f, 0.1f, 1000.0f),
			"perspective"
		);

		MatrixNovice::ScreenPrintf(
			0, kMatrixHeight * 2,
			Matrix::MakeViewport(100.0f, 200.0f, 600.0f, 300.0f, 0.0f, 1.0f),
			"viewport"
		);

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