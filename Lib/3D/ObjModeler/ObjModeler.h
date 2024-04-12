#pragma once

#include <ImGui.h>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Polygon3.h"
#include "Polygon4.h"

class ObjModeler {
public: // menber method //

	ObjModeler();
	~ObjModeler();

private: // menber object //

	Polygon3 polygon3_;
	Polygon4 polygon4_;

	struct Surface3 {
		int v1, v2, v3, v4;
	};

	std::vector<Vector3f> local_;
	std::vector<Surface3> surface_;

public: // user method //

	void ResetModel();

	void LoadObj(
		const std::string& filePath);

	// Draw method //
	void AddModelPolygonLambert(
		const Matrix4x4& worldMatrix, uint32_t color,
		const Camera3D& camera);

	void AddModelPolygonLambert(
		const Matrix4x4& worldMatrix, uint32_t color,
		const Camera3D& camera, const Light& light);

	void AddModelPolygonBlinnPhong(
		const Matrix4x4& worldMatrix, uint32_t color, float specPow,
		const Camera3D& camera, const Light& light);

	void AddModelPolygonBlinnPhong(
		const Matrix4x4& worldMatrix, uint32_t color, float specPow,
		const Camera3D& camera, const LightContainer& lightContainer);


	void Debug();
};

