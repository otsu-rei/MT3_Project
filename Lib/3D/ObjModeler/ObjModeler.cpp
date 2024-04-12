#include "ObjModeler.h"
#include <assert.h>

ObjModeler::ObjModeler() {
}

ObjModeler::~ObjModeler() {
	ResetModel();
}

void ObjModeler::ResetModel() {
	local_.clear();
	surface_.clear();
}

void ObjModeler::LoadObj(const std::string& filePath) {
	ResetModel();

	std::ifstream file(filePath);
	assert(file);

	std::string line;

	while (std::getline(file, line)) {
		std::istringstream iss(line);

		std::string type;
		iss >> type;

		if (type == "v") {

			float x, y, z;
			iss >> x >> y >> z;
			local_.push_back({ x, y, z });


		} else if (type == "f") {
			Surface3 vi;

			iss >> vi.v1;
			iss.ignore(256, ' ');
			iss >> vi.v2;
			iss.ignore(256, ' ');
			iss >> vi.v3;
			iss.ignore(256, ' ');

			if (isdigit(iss.peek())) {
				iss >> vi.v4;

			} else {
				vi.v4 = NULL;
			}

			surface_.push_back(vi);

		}
	}

	file.close();
}

void ObjModeler::AddModelPolygonLambert(
	const Matrix4x4& worldMatrix, uint32_t color,
	const Camera3D& camera) {

	for (const auto& surface : surface_) {
		if (!surface.v4) {
			polygon3_.DrawPolygon3Lambert(
				local_[surface.v1 - 1], local_[surface.v2 - 1], local_[surface.v3 - 1],
				worldMatrix,
				color,
				camera,
				kFillModeSolid
			);

		} else {
			polygon4_.DrawPolygon4Lambert(
				local_[surface.v1 - 1], local_[surface.v2 - 1], local_[surface.v3 - 1], local_[surface.v4 - 1],
				worldMatrix,
				color,
				camera,
				kFillModeSolid
			);
		}
	}
}

void ObjModeler::AddModelPolygonLambert(
	const Matrix4x4& worldMatrix, uint32_t color,
	const Camera3D& camera, const Light& light) {

	for (const auto& surface : surface_) {
		if (!surface.v4) {
			polygon3_.DrawPolygon3Lambert(
				local_[surface.v1 - 1], local_[surface.v2 - 1], local_[surface.v3 - 1],
				worldMatrix,
				color,
				camera,
				light,
				kFillModeSolid
			);

		} else {
			polygon4_.DrawPolygon4Lambert(
				local_[surface.v1 - 1], local_[surface.v2 - 1], local_[surface.v3 - 1], local_[surface.v4 - 1],
				worldMatrix,
				color,
				camera,
				light,
				kFillModeSolid
			);
		}
	}
}


void ObjModeler::AddModelPolygonBlinnPhong(
	const Matrix4x4& worldMatrix, uint32_t color, float specPow,
	const Camera3D& camera, const Light& light) {

	for (const auto& surface : surface_) {
		if (!surface.v4) {
			polygon3_.DrawPolygon3BlinnPhong(
				local_[surface.v1 - 1], local_[surface.v2 - 1], local_[surface.v3 - 1],
				worldMatrix,
				color,
				specPow,
				camera,
				light,
				kFillModeSolid
			);

		} else {
			polygon4_.DrawPolygon4BlinnPhong(
				local_[surface.v1 - 1], local_[surface.v2 - 1], local_[surface.v3 - 1], local_[surface.v4 - 1],
				worldMatrix,
				color,
				specPow,
				camera,
				light,
				kFillModeSolid
			);
		}
	}
}

void ObjModeler::AddModelPolygonBlinnPhong(
	const Matrix4x4& worldMatrix, uint32_t color, float specPow,
	const Camera3D& camera, const LightContainer& lightContainer) {

	for (const auto& surface : surface_) {
		if (!surface.v4) {
			polygon3_.DrawPolygon3BlinnPhong(
				local_[surface.v1 - 1], local_[surface.v2 - 1], local_[surface.v3 - 1],
				worldMatrix,
				color,
				specPow,
				camera,
				lightContainer
			);

		} else {
			polygon4_.DrawPolygon4BlinnPhong(
				local_[surface.v1 - 1], local_[surface.v2 - 1], local_[surface.v3 - 1], local_[surface.v4 - 1],
				worldMatrix,
				color,
				specPow,
				camera,
				lightContainer
			);
		}
	}
}

void ObjModeler::Debug() {
	ImGui::Begin("OBJ");

	int vi = 0;
	for (const auto& vertex : local_) {
		ImGui::Text("%d, x: %f y: %f z: %f", vi, vertex.x, vertex.y, vertex.z);

		vi++;
	}

	vi = 0;
	for (const auto& surface : surface_) {
		ImGui::Text("%d, surface: %d, %d, %d, %d", vi, surface.v1, surface.v2, surface.v3, surface.v4);

		vi++;
	}

	ImGui::End();
}
