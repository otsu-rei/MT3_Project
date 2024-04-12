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
		static_cast<int>(w1.x),
		static_cast<int>(w1.y),
		static_cast<int>(w2.x),
		static_cast<int>(w2.y),
		color
	);

}

const Vector2f& PrimitiveDrawer::ChangeScreenPos(const Vector3f& worldPos) {
	Vector3f result;
	
	Matrix4x4 wvpMatrix = Matrix4x4::MakeIdentity() * camera_->GetViewProjMatrix();
	Vector3f ndcVector = Matrix::Transform(worldPos, wvpMatrix);
	result = Matrix::Transform(ndcVector, camera_->GetViewportMatrix());

	return { result.x, result.y };
}

const Vector2f& PrimitiveDrawer::ChangeScreenPos(const Vector3f& loaclPos, const Matrix4x4& worldMatrix) {
	Vector3f result;

	Matrix4x4 wvpMatrix = worldMatrix * camera_->GetViewProjMatrix();
	Vector3f ndcVector = Matrix::Transform(loaclPos, wvpMatrix);
	result = Matrix::Transform(ndcVector, camera_->GetViewportMatrix());

	return { result.x, result.y };
}
