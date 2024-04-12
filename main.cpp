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

	Vector3f rotate = { 0.4f, 1.43f, -0.8f };

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

		MatrixNovice::ScreenPrintf(0, 0,                 Matrix::MakeRotateX(rotate.x), "rotateXMatrix");
		MatrixNovice::ScreenPrintf(0, kMatrixHeight,     Matrix::MakeRotateX(rotate.y), "rotateYMatrix");
		MatrixNovice::ScreenPrintf(0, kMatrixHeight * 2, Matrix::MakeRotateX(rotate.z), "rotateZMatrix");
		MatrixNovice::ScreenPrintf(0, kMatrixHeight * 3, Matrix::MakeRotate(rotate), "rotateMatrix");

		///
		/// ↓描画処理ここから
		///

		

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