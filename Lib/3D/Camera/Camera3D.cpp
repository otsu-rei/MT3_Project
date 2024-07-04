#include "Camera3D.h"

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
#include <Environment.h>
#include <InputManager.h>
#include <Novice.h>
#include <imgui.h>
#include <MyMath.h>
#include <PrimitiveDrawer.h>

////////////////////////////////////////////////////////////////////////////////////////////
// Camera3D class methods
////////////////////////////////////////////////////////////////////////////////////////////

void Camera3D::Init() {

	transform_.rotate    = { 0.26f, 0.0f, 0.0f };
	transform_.translate = { 0.0f, 1.9f, -6.49f };
	UpdateMatrix();

	SetProjection(0.45f, static_cast<float>(kWindowWidth) / static_cast<float>(kWindowHeight), 0.01f, 1000.0f); // default projection
	SetViewport(0.0f, 0.0f, static_cast<float>(kWindowWidth), static_cast<float>(kWindowHeight), 0.0f, 1.0f); // default viewport

}

void Camera3D::UpdateMatrix() {
	Matrix4x4 cameraMatrix = Matrix::MakeAffine(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = Matrix::Inverse(cameraMatrix);
}

void Camera3D::SetTransform(const Vector3f& scale, const Vector3f& rotate, const Vector3f& translate) {
	transform_.scale = scale;
	transform_.rotate = rotate;
	transform_.translate = translate;
	UpdateMatrix();
}

void Camera3D::SetProjection(
	float fovY, float aspectRatio, float nearClip, float farClip) {
	projectionMatrix_ = Matrix::MakePerspectiveFov(fovY, aspectRatio, nearClip, farClip);
}

void Camera3D::SetViewport(
	float left, float top, float width, float height, float minDepth, float maxDepth) {
	viewportMatrix_ = Matrix::MakeViewport(left, top, width, height, minDepth, maxDepth);
}

void Camera3D::SetOnImGui() {
	if (ImGui::TreeNode("camera")) {

		ImGui::Checkbox("isUseControl", &isUseControl_);

		if (isUseControl_) {

			ImGui::DragFloat3("center", &control_.center.x, 0.02f);
			ImGui::DragFloat("lon", &control_.lon, 0.02f);
			ImGui::DragFloat("lat", &control_.lat, 0.02f);
			ImGui::DragFloat("distance", &control_.distance, 0.02f);

		} else {
			ImGui::DragFloat3("scale", &transform_.scale.x, 0.02f);
			ImGui::DragFloat3("rotate", &transform_.rotate.x, 0.02f);
			ImGui::DragFloat3("translate", &transform_.translate.x, 0.02f);
		}

		ImGui::TreePop();
	}

	if (isUseControl_) {
		UpdateControl();
	}

	UpdateMatrix();
}

void Camera3D::UpdateControl() {
	// parameter //
	const float deltaMove = 0.01f;
	const float deltaRotate = 1.0f / 100.0f;

	/// Update ///
	if (InputManager::IsPressMouse(2)) {
		if (InputManager::IsPressKeys(DIK_LSHIFT)) {
			control_.center += (
				Matrix::Transform(
					{ static_cast<float>(InputManager::GetPreMousePos().x - InputManager::GetMousePos().x) * deltaMove, static_cast<float>(InputManager::GetMousePos().y - InputManager::GetPreMousePos().y) * deltaMove, 0.0f },
					Matrix::MakeRotate(transform_.rotate))
				);

		} else {
			control_.lon += (InputManager::GetMousePos().x - InputManager::GetPreMousePos().x) * deltaRotate;
			control_.lat += (InputManager::GetMousePos().y - InputManager::GetPreMousePos().y) * deltaRotate;
		}

	}

	if (Novice::GetWheel() > 0) { control_.distance -= 0.5f; }
	else if (Novice::GetWheel() < 0) { control_.distance += 0.5f; }

	// translate
	Vector3f point = {
		std::cos(control_.lat) * -std::sin(control_.lon),
		std::sin(control_.lat),
		std::cos(control_.lat) * -std::cos(control_.lon),
	};

	point *= control_.distance;

	transform_.translate = point + control_.center;

	// rotate
	transform_.rotate.x = control_.lat;
	transform_.rotate.y = control_.lon;

	UpdateMatrix();
}

void Camera3D::DrawFrustum(uint32_t color) {

	Vector3f frustumPoint[4];

	Matrix4x4 clipMatrix = Matrix::Inverse(projectionMatrix_);
	Matrix4x4 worldMatrix = Matrix::Inverse(viewMatrix_);

	frustumPoint[0] = Matrix::Transform(Matrix::Transform({-1.0f, -1.0f, 1.0f}, clipMatrix), worldMatrix);
	frustumPoint[1] = Matrix::Transform(Matrix::Transform({1.0f, -1.0f, 1.0f}, clipMatrix), worldMatrix);
	frustumPoint[2] = Matrix::Transform(Matrix::Transform({1.0f, 1.0f, 1.0f}, clipMatrix), worldMatrix);
	frustumPoint[3] = Matrix::Transform(Matrix::Transform({-1.0f, 1.0f, 1.0f}, clipMatrix), worldMatrix);

	auto drawer = PrimitiveDrawer::GetInstance();

	for (int i = 0; i < 4; ++i) {

		drawer->DrawLine(
			frustumPoint[i], frustumPoint[(i + 1) % 4], color
		);

		drawer->DrawLine(
			frustumPoint[i], transform_.translate, color
		);
	}

}
