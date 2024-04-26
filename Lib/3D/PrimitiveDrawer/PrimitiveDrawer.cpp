#include "PrimitiveDrawer.h"

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
#include <numbers>

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


	const float kLatEvery = (std::numbers::pi_v<float> * 2.0f) / static_cast<float>(kSubdivision); // horizontal
	const float kLonEvery = std::numbers::pi_v<float> / static_cast<float>(kSubdivision); // vertical

	enum PointName {
		A, B, C,
	};

	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -std::numbers::pi_v<float> / 2.0f + kLatEvery * latIndex;

		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;

			Vector3f point[3];

			point[A] = { std::cos(lat) * std::cos(lon), std::sin(lat), std::cos(lat) * std::sin(lon) };;
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

//=========================================================================================
// private
//=========================================================================================

Vector2f PrimitiveDrawer::ChangeScreenPos(const Vector3f& worldPos) {
	Vector3f result;
	
	Matrix4x4 wvpMatrix = Matrix4x4::MakeIdentity() * camera_->GetViewProjMatrix();
	Vector3f ndcVector = Matrix::Transform(worldPos, wvpMatrix);
	result = Matrix::Transform(ndcVector, camera_->GetViewportMatrix());

	return { result.x, result.y };
}

Vector2f PrimitiveDrawer::ChangeScreenPos(const Vector3f& loaclPos, const Matrix4x4& worldMatrix) {
	Vector3f result;

	Matrix4x4 wvpMatrix = worldMatrix * camera_->GetViewProjMatrix();
	Vector3f ndcVector = Matrix::Transform(loaclPos, wvpMatrix);
	result = Matrix::Transform(ndcVector, camera_->GetViewportMatrix());

	return { result.x, result.y };
}
