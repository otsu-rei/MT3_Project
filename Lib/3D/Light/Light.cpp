#include "Light.h"

Light::Light() {
	Init();
}

Light::~Light() {
}

void Light::Init() {
	lightMode_ = kLightPoint;

	rotate_ = { 0.0f };
	pos_ = { 0.0f };

	worldMatrix_ = Matrix4x4::MakeAffine({ 1.0f, 1.0f, 1.0f }, rotate_, pos_);

	intensity_ = 1.0f;

	alpha_ = 0xFF;

	pointLight_ = { 0 };
	spotLight_ = { 0 };
	directionalLight_ = { 0 };

	for (int ci = 0; ci < 3; ci++) {
		color_[ci] = 1.0f;
	}
}

void Light::Draw(const Camera3D& camera) const {
	camera.DrawSphere(pos_, 0.15f, 10, 0xf6aa1c00 + alpha_);
	camera.DrawCoordinate(worldMatrix_, 0.2f);
}

void Light::UpdateImGui(const char* windowName, const char* lightName) {
	int lightMode = static_cast<int>(lightMode_);

	ImGui::Begin(windowName);

	ImGui::Separator();

	if (ImGui::TreeNode(lightName)) {
		ImGui::DragFloat3("pos_", &pos_.x, 0.01f);
		ImGui::DragFloat3("rotate_", &rotate_.x, 0.01f);
		worldMatrix_ = Matrix4x4::MakeAffine({1.0f, 1.0f, 1.0f}, rotate_, pos_);

		ImGui::SliderFloat("intensity_", &intensity_, 0.0f, 1.0f);
		/*alpha_ = static_cast<uint32_t>(power_ * 255.0f);*/
		

		ImGui::RadioButton("Point", &lightMode, LightMode::kLightPoint);
		ImGui::SameLine();
		ImGui::RadioButton("Spot", &lightMode, LightMode::kLightSpot);
		ImGui::SameLine();
		ImGui::RadioButton("Directional", &lightMode, LightMode::kLightDirectional);

		lightMode_ = static_cast<LightMode>(lightMode);

		Matrix4x4 rotateMatrix;

		switch (lightMode_) {
			case LightMode::kLightPoint:
				

				if (ImGui::TreeNode("Point Light")) {
					ImGui::DragFloat("range", &pointLight_.range, 0.1f);

					if (pointLight_.range < 0.0f) { pointLight_.range = 0.0f; }

					ImGui::TreePop();
				}

				break;

			case LightMode::kLightSpot:

				rotateMatrix = Matrix4x4::MakeRotate(rotate_);
				spotLight_.direction = Matrix4x4::Transform({ 0.0f, 0.0f, -1.0f }, rotateMatrix);

				if (ImGui::TreeNode("Spot Light")) {
					ImGui::DragFloat("range", &spotLight_.range, 0.1f);
					ImGui::SliderFloat("angle", &spotLight_.angle, -1.0f, 1.0f);

					ImGui::Text("[direction] x: %f, y: %f, z: %f", spotLight_.direction.x, spotLight_.direction.y, spotLight_.direction.z);

					ImGui::TreePop();
				}

				break;

			case LightMode::kLightDirectional:

				rotateMatrix = Matrix4x4::MakeRotate(rotate_);
				directionalLight_.direction = Matrix4x4::Transform({ 0.0f, 0.0f, -1.0f }, rotateMatrix);

				if (ImGui::TreeNode("Directional Light")) {
					
					ImGui::Text("[direction] x: %f, y: %f, z: %f", directionalLight_.direction.x, directionalLight_.direction.y, directionalLight_.direction.z);

					ImGui::TreePop();
				}

				break;
		}
		

		ImGui::TreePop();
	}

	ImGui::Separator();

	ImGui::End();
}
