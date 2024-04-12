#include "Camera3D.h"

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
#include <Environment.h>
#include <imgui.h>

////////////////////////////////////////////////////////////////////////////////////////////
// Camera3D class methods
////////////////////////////////////////////////////////////////////////////////////////////

void Camera3D::Init() {

	transform_.rotate    = { 0.26f, 0.0f, 0.0f };
	transform_.translate = { 0.0f, 1.9f, 10.0f };
	UpdateMatrix();

	SetProjection(0.45f, static_cast<float>(kWindowWidth) / static_cast<float>(kWindowHeight), 5.0f, 1000.0f); // default projection
	SetViewport(0.0f, 0.0f, static_cast<float>(kWindowWidth), static_cast<float>(kWindowHeight), 0.0f, 1.0f); // default viewport

}

void Camera3D::UpdateMatrix() {
	Matrix4x4 cameraMatrix = Matrix::MakeAffine(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = Matrix::Inverse(cameraMatrix);
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
	if (ImGui::TreeNode("todo: cameraName")) {

		ImGui::DragFloat3("scale",     &transform_.scale.x, 0.02f);
		ImGui::DragFloat3("rotate",    &transform_.rotate.x, 0.02f);
		ImGui::DragFloat3("translate", &transform_.translate.x, 0.02f);

		ImGui::TreePop();
	}

	UpdateMatrix();
}
