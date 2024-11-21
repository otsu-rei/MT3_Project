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

	std::unique_ptr<Camera3D> c = std::make_unique<Camera3D>();
	c->SetTransform(unitVector, origin, origin);

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

		if (ImGui::TreeNode("c")) {
			
			c->SetOnImGui();
			ImGui::TreePop();
		}

		ImGui::End();




		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		const Vector2<uint32_t> size = { 20, 30 };

		for (uint32_t w = 0; w < size.x; ++w) {
			for (uint32_t h = 0; h < size.y; ++h) {

				Vector2f uv = {
					static_cast<float>(w) / size.x,
					static_cast<float>(h) / size.y,
				};

				Vector2f d = (uv * 2.0f);
				d.x -= 1.0f;
				d.y -= 1.0f;

				Vector3f t = Matrix::Transform({d.x, -d.y, 1.0f}, Matrix::Inverse(c->GetProjectionMatrix()));
				Vector3f direction = Vector::Normalize(Matrix::TransformNormal(t, Matrix::Transpose(c->GetViewMatrix())));

				Vector3f position = Matrix::Transform(origin, Matrix::Transpose(c->GetViewMatrix()));

				drawer->DrawLine(
					position, position + direction,
					0xFA0000FF
				);
			}
		}

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