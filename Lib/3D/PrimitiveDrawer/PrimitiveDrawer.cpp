#include "PrimitiveDrawer.h"

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
#include <numbers>
#include <MyMath.h>

////////////////////////////////////////////////////////////////////////////////////////////
// PrimitiveDrawer class methods
////////////////////////////////////////////////////////////////////////////////////////////

PrimitiveDrawer* PrimitiveDrawer::GetInstance() {
	static PrimitiveDrawer instance;
	return &instance;
}

void PrimitiveDrawer::DrawLine(
	const Vector3f& w1, const Vector3f& w2, uint32_t color) {

	Vector2f pos[2];
	pos[0] = ChangeScreenPos(w1);
	pos[1] = ChangeScreenPos(w2);

	Novice::DrawLine(
		static_cast<int>(pos[0].x),
		static_cast<int>(pos[0].y),
		static_cast<int>(pos[1].x),
		static_cast<int>(pos[1].y),
		color
	);

}

void PrimitiveDrawer::DrawLine(
	const Vector3f& l1, const Vector3f& l2, const Matrix4x4& worldMatrix, uint32_t color) {

	Vector2f pos[2];
	pos[0] = ChangeScreenPos(l1, worldMatrix);
	pos[1] = ChangeScreenPos(l2, worldMatrix);

	Novice::DrawLine(
		static_cast<int>(pos[0].x),
		static_cast<int>(pos[0].y),
		static_cast<int>(pos[1].x),
		static_cast<int>(pos[1].y),
		color
	);
}

void PrimitiveDrawer::DrawTriangle(
	const Vector3f& l1, const Vector3f& l2, const Vector3f& l3,
	const Matrix4x4& worldMatrix,
	uint32_t color, FillMode fillMode) {

	Vector2f pos[3];
	pos[0] = ChangeScreenPos(l1, worldMatrix);
	pos[1] = ChangeScreenPos(l2, worldMatrix);
	pos[2] = ChangeScreenPos(l3, worldMatrix);

	Novice::DrawTriangle(
		static_cast<int>(pos[0].x),
		static_cast<int>(pos[0].y),
		static_cast<int>(pos[1].x),
		static_cast<int>(pos[1].y),
		static_cast<int>(pos[2].x),
		static_cast<int>(pos[2].y),
		color,
		fillMode
	);

}

void PrimitiveDrawer::DrawTriangle(
	const Vector3f& w1, const Vector3f& w2, const Vector3f& w3,
	uint32_t color, FillMode fillMode) {

	Vector2f pos[3];
	pos[0] = ChangeScreenPos(w1);
	pos[1] = ChangeScreenPos(w2);
	pos[2] = ChangeScreenPos(w3);

	Novice::DrawTriangle(
		static_cast<int>(pos[0].x),
		static_cast<int>(pos[0].y),
		static_cast<int>(pos[1].x),
		static_cast<int>(pos[1].y),
		static_cast<int>(pos[2].x),
		static_cast<int>(pos[2].y),
		color,
		fillMode
	);
}

void PrimitiveDrawer::DrawTriangleCalling(
	const Vector3f& l1, const Vector3f& l2, const Vector3f& l3,
	const Matrix4x4& worldMatrix,
	uint32_t color, FillMode fillMode) {

	Vector3f polygonDirection = Vector::Cross((l2 - l1), (l3 - l2));
	polygonDirection = Matrix::Transform(polygonDirection, worldMatrix);
	polygonDirection = Vector::Normalize(polygonDirection);

	Vector3f cameraDirection = Matrix::Transform({ 0.0f, 0.0f, 1.0f }, Matrix::MakeRotate(camera_->transform_.rotate));

	if (Vector::Dot(cameraDirection, polygonDirection) > 0.0f) { return; }

	DrawTriangle(l1, l2, l3, worldMatrix, color, fillMode);
}

void PrimitiveDrawer::DrawGrid(
	const Vector3f& center,
	float gridLength, const uint32_t kSubdivision,
	uint32_t color) {

	if (kSubdivision == 0) { return; } //!< 描画しないので早期return
	// todo: kSubdivision = 1 の時の処理の追加

	// grid一本の長さ
	float kGridLength = gridLength / static_cast<float>(kSubdivision - 1);

	// grid半分の長さ
	float kGridHalfLength = gridLength / 2.0f;

	for (uint32_t index = 0; index < kSubdivision; ++index) {

		float localGridBorder = kGridHalfLength - (index * kGridLength);

		// z軸方向へのgrid線
		DrawLine(
			{ center.x + localGridBorder, center.y, center.z - kGridHalfLength },
			{ center.x + localGridBorder, center.y, center.z + kGridHalfLength },
			color
		);

		// x軸方法へのgrid線
		DrawLine(
			{ center.x - kGridHalfLength, center.y, center.z + localGridBorder },
			{ center.x + kGridHalfLength, center.y, center.z + localGridBorder },
			color
		);
	}

}

void PrimitiveDrawer::DrawSphere(
	const Vector3f& center, float radius, const uint32_t kSubdivision, uint32_t color) {


	const float kLatEvery = (std::numbers::pi_v<float> *2.0f) / static_cast<float>(kSubdivision); // horizontal
	const float kLonEvery = std::numbers::pi_v<float> / static_cast<float>(kSubdivision); // vertical

	enum PointName {
		A, B, C,
	};

	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -std::numbers::pi_v<float> / 2.0f + kLatEvery * latIndex;

		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;

			Vector3f point[3];

			point[A] = { std::cos(lat) * std::cos(lon), std::sin(lat), std::cos(lat) * std::sin(lon) };
			point[A] *= radius;

			point[B] = {
				std::cos(lat + kLatEvery) * std::cos(lon),
				std::sin(lat + kLatEvery),
				std::cos(lat + kLatEvery) * std::sin(lon)
			};
			point[B] *= radius;

			point[C] = {
				std::cos(lat) * std::cos(lon + kLonEvery),
				std::sin(lat),
				std::cos(lat) * std::sin(lon + kLonEvery),
			};
			point[C] *= radius;

			DrawLine(point[A] + center, point[B] + center, color);
			DrawLine(point[A] + center, point[C] + center, color);

		}
	}
}

void PrimitiveDrawer::DrawPlane(const Plane& plane, uint32_t color) {
	Vector3f center = plane.normal * plane.distance;

	Vector3f perpendiculars[4];
	perpendiculars[0] = Vector::Normalize(Vector::Perpendicular(plane.normal));
	perpendiculars[1] = Vector::Cross(plane.normal, perpendiculars[0]);
	perpendiculars[2] = perpendiculars[0] * -1.0f;
	perpendiculars[3] = perpendiculars[1] * -1.0f;

	Vector3f points[4];
	for (int i = 0; i < 4; ++i) {
		Vector3f extend = perpendiculars[i] * 2.0f;
		points[i] = center + extend;
	}

	for (int i = 0; i < 4; ++i) {
		DrawLine(
			points[i], points[(i + 1) % 4], color
		);
	}

}

void PrimitiveDrawer::DrawAABB(const AABB& aabb, uint32_t color) {
	Vector3f pos[8];

	// A
	for (int i = 0; i < 4; ++i) {
		pos[i] = aabb.min;
	}

	pos[1].z = aabb.max.z;
	pos[2].y = aabb.max.y;
	pos[2].z = aabb.max.z;
	pos[3].y = aabb.max.y;

	// B
	for (int i = 4; i < 8; ++i) {
		pos[i] = aabb.max;
	}

	pos[4].y = aabb.min.y;
	pos[4].z = aabb.min.z;
	pos[5].y = aabb.min.y;
	pos[7].z = aabb.min.z;

	for (int i = 0; i < 4; ++i) {
		int next = (i + 1) % 4;
		DrawLine(pos[i], pos[next], color);
		DrawLine(pos[i + 4], pos[next + 4], color);
		DrawLine(pos[i], pos[i + 4], color);
	}
}

void PrimitiveDrawer::DrawOBB(
	const OBB& obb, uint32_t color) {

	Vector3f pos[8];

	// sizeのみの座標に設定
	pos[0] = { -obb.size.x, obb.size.y, -obb.size.z };
	pos[4] = { -obb.size.x, obb.size.y, obb.size.z };

	pos[1] = { -obb.size.x, -obb.size.y, -obb.size.z };
	pos[5] = { -obb.size.x, -obb.size.y, obb.size.z };

	pos[2] = { obb.size.x, -obb.size.y, -obb.size.z };
	pos[6] = { obb.size.x, -obb.size.y, obb.size.z };

	pos[3] = { obb.size.x, obb.size.y, -obb.size.z };
	pos[7] = { obb.size.x, obb.size.y, obb.size.z };

	for (int i = 0; i < 8; ++i) {
		pos[i] = Matrix::Transform(pos[i], obb.orientation * Matrix::MakeTranslate(obb.center)); //!< 姿勢行列 + centerの適用
	}

	for (int i = 0; i < 4; ++i) {
		int next = (i + 1) % 4;
		DrawLine(pos[i], pos[next], color);
		DrawLine(pos[i + 4], pos[next + 4], color);
		DrawLine(pos[i], pos[i + 4], color);
	}

}

//=========================================================================================
// private
//=========================================================================================

Vector2f PrimitiveDrawer::ChangeScreenPos(const Vector3f& pos, const Matrix4x4& worldMatrix) {
	Vector3f result;

	Matrix4x4 wvpMatrix = worldMatrix * camera_->GetViewProjMatrix();
	Vector3f ndcVector = Matrix::Transform(pos, wvpMatrix);
	result = Matrix::Transform(ndcVector, camera_->GetViewportMatrix());

	return { result.x, result.y };
}