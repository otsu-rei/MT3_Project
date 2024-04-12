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

	Matrix4x4 m1 = {
		3.2f, 0.7f, 9.6f, 4.4f,
		5.5f, 1.3f, 7.8f, 2.1f,
		6.9f, 8.0f, 2.6f, 1.0f,
		0.5f, 7.2f, 5.1f, 3.3f
	};

	Matrix4x4 m2 = {
		4.1f, 6.5f, 3.3f, 2.2f,
		8.8f, 0.6f, 9.9f, 7.7f,
		1.1f, 5.5f, 6.6f, 0.0f,
		3.3f, 9.9f, 8.8f, 2.2f
	};

	const int kMatrixWidth  = MatrixNovice::kColumnWidth * 5 + 20;
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

		MatrixNovice::ScreenPrintf(0, 0,                 m1 + m2, "Add");
		MatrixNovice::ScreenPrintf(0, kMatrixHeight,     m1 - m2, "Subtract");
		MatrixNovice::ScreenPrintf(0, kMatrixHeight * 2, m1 * m2, "Multiply");
		MatrixNovice::ScreenPrintf(0, kMatrixHeight * 3, Matrix::Inverse(m1), "Inverse(m1)");
		MatrixNovice::ScreenPrintf(0, kMatrixHeight * 4, Matrix::Inverse(m2), "Inverse(m2)");

		MatrixNovice::ScreenPrintf(kMatrixWidth, 0,                 Matrix::Transpose(m1), "Transpose(m1)");
		MatrixNovice::ScreenPrintf(kMatrixWidth, kMatrixHeight,     Matrix::Transpose(m2), "Transpose(m2)");
		MatrixNovice::ScreenPrintf(kMatrixWidth, kMatrixHeight * 2, Matrix4x4::MakeIdentity(), "identity");

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