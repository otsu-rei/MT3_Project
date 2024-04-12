#include "PrimitiveDrawer.h"

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
