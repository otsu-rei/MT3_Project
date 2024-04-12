#include "Polygon3.h"

namespace {
	float Clamp(float value, float min, float max) {
		value = value > max ? max : value;
		value = value < min ? min : value;

		return value;
	}
}

Polygon3::Polygon3() {
}

Polygon3::~Polygon3() {
}


//-----------------------------------------------------------------------------------------
// public method
//-----------------------------------------------------------------------------------------
void Polygon3::DrawPolygon3(
	const Vector3f& localPosA, const Vector3f& localPosB, const Vector3f& localPosC,
	const Matrix4x4& worldMatrix, uint32_t color,
	const Camera3D& camera, uint16_t layer,
	FillMode fillMode, BlendMode blendMode) {

	Vector3f localPos[kVertexNum_] = {
		localPosA,
		localPosB,
		localPosC,
	};

	SetPolygonData(localPos, worldMatrix);
	SetScreenPos(localPos, worldMatrix, camera);
	SetCameraDot(camera);


	if (polygon2cameraPosDot_ >= 0.0f && polygon2cameraDirectionDot_ < 0.0f) {
		displayColor_ = color;

		AddPolygonElement(layer, fillMode, blendMode);
	}
}


void Polygon3::DrawPolygon3Lambert(
	const Vector3f& localPosA, const Vector3f& localPosB, const Vector3f& localPosC,
	const Matrix4x4& worldMatrix, uint32_t color,
	const Camera3D& camera, uint16_t layer,
	FillMode fillMode, BlendMode blendMode) {

	Vector3f localPos[kVertexNum_] = {
		localPosA,
		localPosB,
		localPosC,
	};

	SetPolygonData(localPos, worldMatrix);
	SetScreenPos(localPos, worldMatrix, camera);
	SetCameraDot(camera);

	if (polygon2cameraPosDot_ >= 0.0f && polygon2cameraDirectionDot_ < 0.0f) {
		lightDirection_ = cameraDirection_;
		polygon2lightDot_ = polygon2cameraPosDot_;

		lightAmount_ = 1.0f;

		FixColorLambert(color);

		AddPolygonElement(layer, fillMode, blendMode);
	}
}

void Polygon3::DrawPolygon3Lambert(
	const Vector3f& localPosA, const Vector3f& localPosB, const Vector3f& localPosC,
	const Matrix4x4& worldMatrix, uint32_t color,
	const Camera3D& camera, const Light& light, uint16_t layer,
	FillMode fillMode, BlendMode blendMode) {

	Vector3f localPos[kVertexNum_] = {
		localPosA,
		localPosB,
		localPosC,
	};

	SetPolygonData(localPos, worldMatrix);
	SetScreenPos(localPos, worldMatrix, camera);
	SetCameraDot(camera);

	if (polygon2cameraPosDot_ >= 0.0f && polygon2cameraDirectionDot_ < 0.0f) {
		SetLightDot(light);
		SetLightAmount(light);

		FixColorLambert(color, light);

		AddPolygonElement(layer, fillMode, blendMode);
	}
}

void Polygon3::DrawPolygon3BlinnPhong(
	const Vector3f& localPosA, const Vector3f& localPosB, const Vector3f& localPosC,
	const Matrix4x4& worldMatrix, uint32_t color, float specPow,
	const Camera3D& camera, const Light& light, uint16_t layer,
	FillMode fillMode, BlendMode blendMode) {

	Vector3f localPos[kVertexNum_] = {
		localPosA,
		localPosB,
		localPosC,
	};

	SetPolygonData(localPos, worldMatrix);
	SetScreenPos(localPos, worldMatrix, camera);
	SetCameraDot(camera);

	if (polygon2cameraPosDot_ >= 0.0f && polygon2cameraDirectionDot_ < 0.0f) {
		SetLightDot(light);
		SetLightAmount(light);

		FixColorBlinnPhong(color, specPow, light);

		AddPolygonElement(layer, fillMode, blendMode);
	}
}

void Polygon3::DrawPolygon3Lambert(
	const Vector3f& localPosA, const Vector3f& localPosB, const Vector3f& localPosC,
	const Matrix4x4& worldMatrix, uint32_t color,
	const Camera3D& camera, const LightContainer& lightContainer, uint16_t layer,
	FillMode fillMode, BlendMode blendMode) {

	Vector3f localPos[kVertexNum_] = {
		localPosA,
		localPosB,
		localPosC,
	};

	SetPolygonData(localPos, worldMatrix);
	SetScreenPos(localPos, worldMatrix, camera);
	SetCameraDot(camera);

	if (polygon2cameraPosDot_ >= 0.0f && polygon2cameraDirectionDot_ < 0.0f) {
		SetLightDot(lightContainer);
		SetLightAmount(lightContainer);

		FixColorLambert(color, lightContainer);

		AddPolygonElement(layer, fillMode, blendMode);
	}

	Finalize();
}

void Polygon3::DrawPolygon3BlinnPhong(
	const Vector3f& localPosA, const Vector3f& localPosB, const Vector3f& localPosC,
	const Matrix4x4& worldMatrix, uint32_t color, float specPow,
	const Camera3D& camera, const LightContainer& lightContainer, uint16_t layer,
	FillMode fillMode, BlendMode blendMode) {

	Vector3f localPos[kVertexNum_] = {
		localPosA,
		localPosB,
		localPosC,
	};

	SetPolygonData(localPos, worldMatrix);
	SetScreenPos(localPos, worldMatrix, camera);
	SetCameraDot(camera);

	if (polygon2cameraPosDot_ >= 0.0f && polygon2cameraDirectionDot_ < 0.0f) {
		SetLightDot(lightContainer);
		SetLightAmount(lightContainer);

		FixColorBlinnPhong(color, specPow, lightContainer);

		AddPolygonElement(layer, fillMode, blendMode);
	}

	Finalize();
}

//-----------------------------------------------------------------------------------------
// private method
//-----------------------------------------------------------------------------------------
void Polygon3::Finalize() {
	polygon2lightDotMap_.clear();
	lightAmountMap_.clear();
	lightDirectionMap_.clear();
}

/* -----------------------------------------------------------------------------------------
 polygon
----------------------------------------------------------------------------------------- */
void Polygon3::SetPolygonData(
	const Vector3f localPos[kVertexNum_], const Matrix4x4& worldMatrix) {

	/* ポリゴンの中心座標を求める */
	Vector3f localCenterPos = { 0.0f };
	for (int vi = 0; vi < kVertexNum_; vi++) {
		localCenterPos += localPos[vi];
	}

	localCenterPos.x /= kVertexNum_;
	localCenterPos.y /= kVertexNum_;
	localCenterPos.z /= kVertexNum_;

	worldCenterPos_ = Matrix4x4::Transform(localCenterPos, worldMatrix);

	/* ポリゴンの法線ベクトルを求める */
	Vector3f kWorldVertices[3];

	for (int vi = 0; vi < kVertexNum_; vi++) {
		kWorldVertices[vi] = Matrix4x4::Transform(localPos[vi], worldMatrix);
	}


	Vector3f verticesVector[2];

	for (int vi = 0; vi < kVertexNum_ - 1; vi++) {
		verticesVector[vi] = Vector::CalculateDirection(kWorldVertices[vi], kWorldVertices[vi + 1]);
	}

	polygonDirection_ = Vector::CrossProduct(verticesVector[A], verticesVector[B]);
	Vector::Normalize(polygonDirection_);
}

/* -----------------------------------------------------------------------------------------
 camera
----------------------------------------------------------------------------------------- */
void Polygon3::SetScreenPos(
	const Vector3f localPos[kVertexNum_], const Matrix4x4& worldMatrix, const Camera3D& camera) {

	Matrix4x4 wvpMatrix = worldMatrix * camera.GetViewProjectionMatrix();

	Vector3f ndcVector;
	for (int vi = 0; vi < kVertexNum_; vi++) {
		ndcVector = Matrix4x4::Transform(localPos[vi], wvpMatrix);
		screenVerticesPos_[vi] = Matrix4x4::Transform(ndcVector, camera.GetViewportMatrix());
	}
}

void Polygon3::SetCameraDot(
	const Camera3D& camera) {

	cameraDirection_ = Vector::CalculateDirection(worldCenterPos_, camera.GetCameraTranslate());
	Vector::Normalize(cameraDirection_);

	polygon2cameraPosDot_ = Vector::DotProduct(cameraDirection_, polygonDirection_);
	polygon2cameraDirectionDot_ = Vector::DotProduct(cameraDirection_, camera.GetCameraDirection());

	distance_ = Vector::CalculateLength(worldCenterPos_, camera.GetCameraTranslate());

	// pre distance
	Matrix4x4 wvpMatrix = Matrix4x4::MakeAffine({1.0f, 1.0f, 1.0f}, {0.0f}, {0.0f}) * camera.GetViewProjectionMatrix();
	Vector3f ndcDistance = Matrix4x4::Transform(worldCenterPos_, wvpMatrix);

	/*distance_ = ndcDistance.z;*/
}

/* -----------------------------------------------------------------------------------------
 light
----------------------------------------------------------------------------------------- */
void Polygon3::SetLightDot(
	const Light& light) {

	if (light.lightMode_ == LightMode::kLightDirectional) {
		lightDirection_ = light.GetDirectionalLightSetting().direction;
		polygon2lightDot_ = Vector::DotProduct(lightDirection_, polygonDirection_);

	} else {
		lightDirection_ = Vector::CalculateDirection(worldCenterPos_, light.GetPos());
		Vector::Normalize(lightDirection_);

		polygon2lightDot_ = Vector::DotProduct(lightDirection_, polygonDirection_);
	}
}

void Polygon3::SetLightAmount(
	const Light& light) {

	float length = 0.0f;

	switch (light.lightMode_) {
		case LightMode::kLightPoint:

			lightAmount_ = 0.0f;
			length = Vector::CalculateLength(worldCenterPos_, light.GetPos());

			if (length < light.GetPointLightSetting().range) {
				lightAmount_ = std::powf((light.GetPointLightSetting().range - length) / light.GetPointLightSetting().range, 2);
			}

			break;

		case LightMode::kLightSpot:

			// TODO: ---
			lightAmount_ = 1.0f;

			break;

		case LightMode::kLightDirectional:

			lightAmount_ = light.GetIntensity();

			break;
	}
}

void Polygon3::SetLightDot(
	const LightContainer& lightContainer) {

	float polygon2lightDot = 0.0f;
	Vector3f lightDirection = { 0.0f };

	for (const auto& light : lightContainer.GetLightContainer()) {
		if (light.second->lightMode_ == LightMode::kLightDirectional) {
			lightDirection = light.second->GetDirectionalLightSetting().direction;
			polygon2lightDot = Vector::DotProduct(lightDirection, polygonDirection_);

		} else {
			lightDirection = Vector::CalculateDirection(worldCenterPos_, light.second->GetPos());
			Vector::Normalize(lightDirection);

			polygon2lightDot = Vector::DotProduct(lightDirection, polygonDirection_);
		}

		polygon2lightDot = Clamp(polygon2lightDot, 0.0f, 1.0f);

		polygon2lightDotMap_[light.first] = polygon2lightDot;
		lightDirectionMap_[light.first] = lightDirection;
	}
}

void Polygon3::SetLightAmount(
	const LightContainer& lightContainer) {

	for (const auto& light : lightContainer.GetLightContainer()) {
		float length = 0.0f;
		float lightAmount = 0.0f;

		switch (light.second->lightMode_) {
			case LightMode::kLightPoint:

				length = Vector::CalculateLength(worldCenterPos_, light.second->GetPos());

				if (length < light.second->GetPointLightSetting().range) {
					lightAmount = std::powf((light.second->GetPointLightSetting().range - length) / light.second->GetPointLightSetting().range, 2);
				}

				break;

			case LightMode::kLightSpot:

				// TODO: ---
				lightAmount = 1.0f;

				break;

			case LightMode::kLightDirectional:

				lightAmount = light.second->GetIntensity();

				break;
		}

		lightAmount = Clamp(lightAmount, 0.0f, 1.0f);
		lightAmountMap_[light.first] = lightAmount;
	}
}


/* -----------------------------------------------------------------------------------------
 fix color
----------------------------------------------------------------------------------------- */
/* --- camera only --- */
void Polygon3::FixColorLambert(
	uint32_t color) {

	polygon2lightDot_ = Clamp(polygon2lightDot_, 0.0f, 1.0f);

	// display color
	float colorValue[3];

	for (int ci = 0; ci < 3; ci++) {
		colorValue[ci] = static_cast<float>((color >> (8 * (3 - ci))) & 0xFF);
	}

	for (int ci = 0; ci < 3; ci++) {
		colorValue[ci] *= lightAmount_;
		colorValue[ci] = (polygon2lightDot_ * colorValue[ci]) * 1.0f;
		// TODO: light power & length remove

		if (colorValue[ci] < 0.0f) { colorValue[ci] = 0.0f; } else if (colorValue[ci] > 255.0f) { colorValue[ci] = 255.0f; }
	}


	displayColor_ = 0; // display color reset

	for (int ci = 0; ci < 3; ci++) {
		displayColor_ += static_cast<uint32_t>(static_cast<int>(colorValue[ci]) << (8 * (3 - ci)));
	}

	displayColor_ += color & 0x000000FF;
}

/* --- light --- */
void Polygon3::FixColorLambert(
	uint32_t color, const Light& light) {

	polygon2lightDot_ = Clamp(polygon2lightDot_, 0.0f, 1.0f);

	// display color
	float colorValue[3];

	for (int ci = 0; ci < 3; ci++) {
		colorValue[ci] = static_cast<float>((color >> (8 * (3 - ci))) & 0xFF);
	}

	for (int ci = 0; ci < 3; ci++) {
		colorValue[ci] *= lightAmount_;
		colorValue[ci] = (polygon2lightDot_ * colorValue[ci]) * light.GetColor(static_cast<LightColor>(ci));
		// TODO: light power & length remove

		if (colorValue[ci] < 0.0f) { colorValue[ci] = 0.0f; } else if (colorValue[ci] > 255.0f) { colorValue[ci] = 255.0f; }
	}


	displayColor_ = 0; // display color reset

	for (int ci = 0; ci < 3; ci++) {
		displayColor_ += static_cast<uint32_t>(static_cast<int>(colorValue[ci]) << (8 * (3 - ci)));
	}

	displayColor_ += color & 0x000000FF;
}

void Polygon3::FixColorBlinnPhong(
	uint32_t color, float specPow, const Light& light) {

	Vector3f blinnPhongDirection = lightDirection_ + cameraDirection_;
	Vector::Normalize(blinnPhongDirection);

	float blinnPhongDot = Vector::DotProduct(blinnPhongDirection, polygonDirection_);

	// Half Lambert
	polygon2lightDot_ = Clamp(polygon2lightDot_, 0.0f, 1.0f);
	blinnPhongDot = Clamp(blinnPhongDot, 0.0f, 1.0f);

	float spec = std::pow(blinnPhongDot, specPow); // specPower

	// display color
	float colorValue[3];

	for (int ci = 0; ci < 3; ci++) {
		colorValue[ci] = static_cast<float>((color >> (8 * (3 - ci))) & 0xFF) / 255.0f;

	}

	for (int ci = 0; ci < 3; ci++) {
		float lambertColor
			= ((polygon2lightDot_ * colorValue[ci]) * lightAmount_) * light.GetColor(static_cast<LightColor>(ci));

		float blinnPhangColor
			= (spec * light.GetColor(static_cast<LightColor>(ci))) * lightAmount_;

		colorValue[ci] = lambertColor + blinnPhangColor;

		if (colorValue[ci] > 1.0f) { colorValue[ci] = 1.0f; } else if (colorValue[ci] < 0.0f) { colorValue[ci] = 0.0f; }

		colorValue[ci] *= 255.0f;



		if (colorValue[ci] > 255.0f) { colorValue[ci] = 255.0f; }
		/*else if (colorValue[ci] < 0.0f) { colorValue[ci] = 0.0f; }*/
	}


	displayColor_ = 0; // display color reset

	for (int ci = 0; ci < 3; ci++) {
		displayColor_ += static_cast<uint32_t>(static_cast<int>(colorValue[ci]) << (8 * (3 - ci)));
	}

	displayColor_ += color & 0x000000FF;
	return;
}

/* --- light container --- */
void Polygon3::FixColorLambert(
	uint32_t color, const LightContainer& lightContainer) {

	polygon2lightDot_ = Clamp(polygon2lightDot_, 0.0f, 1.0f);

	// display color
	float colorValue[3];

	for (int ci = 0; ci < 3; ci++) {
		colorValue[ci] = static_cast<float>((color >> (8 * (3 - ci))) & 0xFF);
	}

	float displayColorValue[3] = { 0.0f };

	for (const auto& light : lightContainer.GetLightContainer()) {
		for (int ci = 0; ci < 3; ci++) {
			displayColorValue[ci]
				+= (polygon2lightDotMap_[light.first] * (colorValue[ci] * lightAmountMap_[light.first])) * light.second->GetColor(static_cast<LightColor>(ci));
		}
	}

	for (int ci = 0; ci < 3; ci++) {
		Clamp(displayColorValue[ci], 0.0f, 255.0f);
	}

	displayColor_ = 0; // display color reset

	for (int ci = 0; ci < 3; ci++) {
		displayColor_ += static_cast<uint32_t>(static_cast<int>(displayColorValue[ci]) << (8 * (3 - ci)));
	}

	displayColor_ += color & 0x000000FF;
}

void Polygon3::FixColorBlinnPhong(
	uint32_t color, float specPow, const LightContainer& lightContainer) {

	// display color
	float colorValue[3];
	float displayColorValue[3] = { 0.0f };

	for (int ci = 0; ci < 3; ci++) {
		colorValue[ci] = static_cast<float>((color >> (8 * (3 - ci))) & 0xFF) / 255.0f;
	}

	for (const auto& light : lightContainer.GetLightContainer()) {
		Vector3f blinnPhongDirection = lightDirectionMap_[light.first] + cameraDirection_;
		Vector::Normalize(blinnPhongDirection);

		float blinnPhongDot = Vector::DotProduct(blinnPhongDirection, polygonDirection_);

		// Half Lambert
		blinnPhongDot = Clamp(blinnPhongDot, 0.0f, 1.0f);
		float spec = std::pow(blinnPhongDot, specPow); // specPower

		for (int ci = 0; ci < 3; ci++) {
			float lambertColor
				= ((polygon2lightDotMap_[light.first] * colorValue[ci]) * lightAmountMap_[light.first]) * light.second->GetColor(static_cast<LightColor>(ci));

			float blinnPhangColor
				= (spec * light.second->GetColor(static_cast<LightColor>(ci))) * lightAmountMap_[light.first];

			displayColorValue[ci] += lambertColor + blinnPhangColor;

		}
	}

	for (int ci = 0; ci < 3; ci++) {
		if (displayColorValue[ci] > 1.0f) { displayColorValue[ci] = 1.0f; } else if (displayColorValue[ci] < 0.0f) { displayColorValue[ci] = 0.0f; }

		displayColorValue[ci] *= 255.0f;

		if (displayColorValue[ci] > 255.0f) { displayColorValue[ci] = 255.0f; }
		/*else if (displayColorValue[ci] < 0.0f) { displayColorValue[ci] = 0.0f; }*/
	}

	displayColor_ = 0; // display color reset

	for (int ci = 0; ci < 3; ci++) {
		displayColor_ += static_cast<uint32_t>(static_cast<int>(displayColorValue[ci]) << (8 * (3 - ci)));
	}

	displayColor_ += color & 0x000000FF;
	return;
}


//-----------------------------------------------------------------------------------------
// manager method
//-----------------------------------------------------------------------------------------
void Polygon3::AddPolygonElement(
	uint16_t layer, FillMode fillMode, BlendMode blendMode) {

	SortPolygonElement element;
	element.polygonType = PolygonType::kPolygonTriangle;

	for (int vi = 0; vi < kVertexNum_; vi++) {
		element.screenPos[vi] = screenVerticesPos_[vi];
	}

	element.color = displayColor_;

	element.fillMode = fillMode;
	element.blendMode = blendMode;

	element.distance = distance_;
	element.layer = layer;

	manager->AddPolygonElement(element);
}
