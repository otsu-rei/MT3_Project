#include "Camera3D.h"

/* -----------------------------------------------------------------------------------------
 construct & destruct
----------------------------------------------------------------------------------------- */
Camera3D::Camera3D() {
	Init();
}

Camera3D::~Camera3D() {
}

/* -----------------------------------------------------------------------------------------
 Initialize
----------------------------------------------------------------------------------------- */
void Camera3D::Init() {
	camera_ = { 0.0f };

	SetCamera({ 1.0f, 1.0f, 1.0f }, { 0.26f, 0.0f, 0.0f }, { 0.0f, 1.9f, 10.0f }); // default camera
	SetProjection(0.45f, static_cast<float>(kWindowWidth) / static_cast<float>(kWindowHeight), 5.0f, 1000.0f); // default projection
	SetViewport(0.0f, 0.0f, static_cast<float>(kWindowWidth), static_cast<float>(kWindowHeight), 0.0f, 1.0f); // default viewport

	// camera control
	controlPoint_ = { 0.0f };
	lon_ = 0.0f;
	lat_ = 0.0f;
	distance_ = 10.0f;
}

/* -----------------------------------------------------------------------------------------
 Update camera data
----------------------------------------------------------------------------------------- */
void Camera3D::UpdateCameraData() {
	Matrix4x4 cameraMatrix = Matrix4x4::MakeAffine(camera_.scale, camera_.rotate, camera_.translate);
	viewMatrix_ = Matrix4x4::Inverse(cameraMatrix);

	direction_ = Matrix4x4::Transform({ 0.0f, 0.0f, 1.0f }, Matrix4x4::MakeRotate(camera_.rotate));
}

/* -----------------------------------------------------------------------------------------
 grapics pipeline // plan to delete
----------------------------------------------------------------------------------------- */
Vector3f Camera3D::CheckGraphicPipeline(
	const Vector3f& localPos, const Matrix4x4& worldMatrix, const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix, const Matrix4x4& viewportMatrix) {

	Vector3f result;

	Matrix4x4 wvpMatrix = worldMatrix * (viewMatrix * projectionMatrix);
	Vector3f ndcVector = Matrix4x4::Transform(localPos, wvpMatrix);
	result = Matrix4x4::Transform(ndcVector, viewportMatrix);

	return result;
}

Vector3f Camera3D::CheckGraphicPipeline(const Vector3f& localPos, const Matrix4x4& worldMatrix) {
	Vector3f result;

	Matrix4x4 wvpMatrix = worldMatrix * (viewMatrix_ * projectionMatrix_);
	Vector3f ndcVector = Matrix4x4::Transform(localPos, wvpMatrix);
	result = Matrix4x4::Transform(ndcVector, viewportMatrix_);

	return result;
}

/* -----------------------------------------------------------------------------------------
 ImGui Editer
----------------------------------------------------------------------------------------- */
void Camera3D::SetCameraImGui(
	const char* windowName, const char* cameraName) {

	// ImGui //
	ImGui::Begin(windowName);

	if (ImGui::TreeNode(cameraName)) {
		ImGui::Text("setter // Camera3D");
		ImGui::DragFloat3("cameraScale", &camera_.scale.x, 0.01f);
		ImGui::DragFloat3("cameraRotate", &camera_.rotate.x, 0.01f);
		ImGui::DragFloat3("cameraTranslate", &camera_.translate.x, 0.01f);

		//ImGui::SliderFloat3("cameraScale", &camera_.scale.x, 0.0001f, 0.01f);

		ImGui::TreePop();
	}

	ImGui::End();

	UpdateCameraData();
}

/* -----------------------------------------------------------------------------------------
 seter
----------------------------------------------------------------------------------------- */
void Camera3D::SetCamera(
	const Vector3f& scale, const Vector3f& rotate, const Vector3f& translate) {


	camera_.scale = scale;
	camera_.rotate = rotate;
	camera_.translate = translate;

	UpdateCameraData();
}

void Camera3D::SetCamera(
	const Vector3f& translate) {

	camera_.translate = translate;

	UpdateCameraData();
}

void Camera3D::SetProjection(
	float fovY, float aspectRatio, float nearClip, float farClip) {
	projectionMatrix_ = Matrix4x4::MakePerspectiveFov(fovY, aspectRatio, nearClip, farClip);
}

void Camera3D::SetViewport(
	float left, float top, float width, float height, float minDepth, float maxDepth) {

	viewportMatrix_ = Matrix4x4::MakeViewport(left, top, width, height, minDepth, maxDepth);
}

/* -----------------------------------------------------------------------------------------
 update camera using mouse
----------------------------------------------------------------------------------------- */
void Camera3D::UpdateCamera() { // rotate center control only

	/// Update ///
	if (InputManager::IsPressMouse(2)) {
		if (InputManager::IsPressKeys(DIK_LSHIFT)) {
			controlPoint_ += (
				Matrix4x4::Transform(
					{ static_cast<float>(InputManager::GetPreMousePos().x - InputManager::GetMousePos().x) * 0.01f, static_cast<float>(InputManager::GetMousePos().y - InputManager::GetPreMousePos().y) * 0.01f, 0.0f},
					Matrix4x4::MakeRotate(camera_.rotate))
				);

		} else {
			lon_ += (InputManager::GetMousePos().x - InputManager::GetPreMousePos().x) / 100.0f;
			lat_ += (InputManager::GetMousePos().y - InputManager::GetPreMousePos().y) / 100.0f;
		}
	}

	if (Novice::GetWheel() > 0) { distance_ -= 0.5f; }
	else if (Novice::GetWheel() < 0) { distance_ += 0.5f; }


	// translate
	Vector3f t = {
		std::cos(lat_) * -std::sin(lon_),
		std::sin(lat_),
		std::cos(lat_) * -std::cos(lon_),
	};

	t = t * distance_;

	camera_.translate = t + controlPoint_;

	// rotate
	camera_.rotate.x = lat_;
	camera_.rotate.y = lon_;

	UpdateCameraData();
}

/* -----------------------------------------------------------------------------------------
 set camera view point
----------------------------------------------------------------------------------------- */
void Camera3D::SetCameraPoint(
	const Vector3f& point, float lon, float lat, float distance) {

	controlPoint_ = point;
	lon_ = lon;
	lat_ = lat;
	distance_ = distance;

	Vector3f t = {
		std::cos(lat) * -std::sin(lon),
		std::sin(lat),
		std::cos(lat) * -std::cos(lon),
	};

	t = t * distance;

	camera_.translate = t + point;

	// rotate
	camera_.rotate.x = lat;
	camera_.rotate.y = lon;

	UpdateCameraData();
}

/* -----------------------------------------------------------------------------------------
 camera draw debug
----------------------------------------------------------------------------------------- */
void Camera3D::DrawGrid(
	float gridHalfLength, uint32_t kSubdivision, uint32_t color, AxisType axisType) const {

	const float kGridEvery = (gridHalfLength * 2.0f) / static_cast<float>(kSubdivision);
	Vector3f pos[2];
	Matrix4x4 pointWorldMatrix = Matrix4x4::MakeAffine({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	Matrix4x4 worldViewProjectionMatrix;
	Vector3f ndcPos;

	// z line
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {

		pos[0] = { kGridEvery * (static_cast<float>(xIndex) - (kSubdivision / 2.0f)), 0.0f, -gridHalfLength };
		pos[1] = { kGridEvery * (static_cast<float>(xIndex) - (kSubdivision / 2.0f)), 0.0f, gridHalfLength };

		for (int li = 0; li < 2; li++) {
			worldViewProjectionMatrix = pointWorldMatrix * (viewMatrix_ * projectionMatrix_);

			ndcPos = Matrix4x4::Transform(pos[li], worldViewProjectionMatrix);
			pos[li] = Matrix4x4::Transform(ndcPos, viewportMatrix_);
		}

		if (axisType == AxisType::IN_COLOR && static_cast<float>(xIndex) - (kSubdivision / 2.0f) == 0.0f) {
			Novice::DrawLine(
				static_cast<int>(pos[0].x), static_cast<int>(pos[0].y),
				static_cast<int>(pos[1].x), static_cast<int>(pos[1].y),
				0x0000FAFF
			);

			continue;
		}

		Novice::DrawLine(
			static_cast<int>(pos[0].x), static_cast<int>(pos[0].y),
			static_cast<int>(pos[1].x), static_cast<int>(pos[1].y),
			color
		);
	}

	// x line
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		pos[0] = { -gridHalfLength, 0.0f, kGridEvery * (static_cast<float>(zIndex) - (kSubdivision / 2.0f)) };
		pos[1] = { gridHalfLength, 0.0f, kGridEvery * (static_cast<float>(zIndex) - (kSubdivision / 2.0f)) };

		for (int li = 0; li < 2; li++) {
			worldViewProjectionMatrix = pointWorldMatrix * (viewMatrix_ * projectionMatrix_);

			ndcPos = Matrix4x4::Transform(pos[li], worldViewProjectionMatrix);
			pos[li] = Matrix4x4::Transform(ndcPos, viewportMatrix_);
		}

		if (axisType == AxisType::IN_COLOR && static_cast<float>(zIndex) - (kSubdivision / 2.0f) == 0.0f) {
			Novice::DrawLine(
				static_cast<int>(pos[0].x), static_cast<int>(pos[0].y),
				static_cast<int>(pos[1].x), static_cast<int>(pos[1].y),
				0xFA0000FF
			);

			continue;
		}

		Novice::DrawLine(
			static_cast<int>(pos[0].x), static_cast<int>(pos[0].y),
			static_cast<int>(pos[1].x), static_cast<int>(pos[1].y),
			color
		);
	}
}

void Camera3D::DrawSphere(
	const Vector3f& center, float radius, uint32_t kSubdivision, uint32_t color) const {

	const float kLatEvery = (static_cast<float>(M_PI) * 2.0f) / static_cast<float>(kSubdivision); // horizontal
	const float kLonEvery = static_cast<float>(M_PI) / static_cast<float>(kSubdivision); // vertical

	enum PointName {
		A,
		B,
		C,
	};

	Matrix4x4 worldMatrix = Matrix4x4::MakeAffine({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, center);

	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -static_cast<float>(M_PI) / 2.0f + kLatEvery * latIndex;

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

			for (int pi = 0; pi < 3; pi++) {
				point[pi] = CheckGraphicPipeline(
					point[pi], worldMatrix,
					viewMatrix_, projectionMatrix_, viewportMatrix_
				);
			}

			Novice::DrawLine(
				static_cast<int>(point[A].x), static_cast<int>(point[A].y),
				static_cast<int>(point[B].x), static_cast<int>(point[B].y),
				color
			);

			Novice::DrawLine(
				static_cast<int>(point[A].x), static_cast<int>(point[A].y),
				static_cast<int>(point[C].x), static_cast<int>(point[C].y),
				color
			);
		}
	}
}

void Camera3D::DrawCoordinate(
	const Vector3f& center, float length) const {

	Matrix4x4 worldMatrix = Matrix4x4::MakeAffine({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, center); // TODO:

	Vector3f localCoordinatePoint[3] = {
		{length, 0.0f, 0.0f}, // x coordinate
		{0.0f, length, 0.0f}, // y coordinate
		{0.0f, 0.0f, length}, // z coordinate
	};

	Vector3f screenCenterPos = CheckGraphicPipeline(
		{ 0.0f }, worldMatrix, viewMatrix_, projectionMatrix_, viewportMatrix_
	);

	Vector3f screenCoordinatePos[3];
	for (int ci = 0; ci < 3; ci++) {
		screenCoordinatePos[ci] = CheckGraphicPipeline(
			localCoordinatePoint[ci], worldMatrix, viewMatrix_, projectionMatrix_, viewportMatrix_
		);
	}

	uint32_t color[3] = {
		0xFA0000FF,
		0x00FA00FF,
		0x0000FAFF
	};

	for (int ci = 0; ci < 3; ci++) {
		Novice::DrawLine(
			static_cast<int>(screenCenterPos.x), static_cast<int>(screenCenterPos.y),
			static_cast<int>(screenCoordinatePos[ci].x), static_cast<int>(screenCoordinatePos[ci].y),
			color[ci]
		);
	}
}

void Camera3D::DrawCoordinate(
	const Matrix4x4& worldMatrix, float length) const {

	Vector3f localCoordinatePoint[3] = {
		{length, 0.0f, 0.0f}, // x coordinate
		{0.0f, length, 0.0f}, // y coordinate
		{0.0f, 0.0f, length}, // z coordinate
	};

	Vector3f screenCenterPos = CheckGraphicPipeline(
		{ 0.0f }, worldMatrix, viewMatrix_, projectionMatrix_, viewportMatrix_
	);

	Vector3f screenCoordinatePos[3];
	for (int ci = 0; ci < 3; ci++) {
		screenCoordinatePos[ci] = CheckGraphicPipeline(
			localCoordinatePoint[ci], worldMatrix, viewMatrix_, projectionMatrix_, viewportMatrix_
		);
	}

	uint32_t color[3] = {
		0xFA0000FF,
		0x00FA00FF,
		0x0000FAFF
	};

	for (int ci = 0; ci < 3; ci++) {
		Novice::DrawLine(
			static_cast<int>(screenCenterPos.x), static_cast<int>(screenCenterPos.y),
			static_cast<int>(screenCoordinatePos[ci].x), static_cast<int>(screenCoordinatePos[ci].y),
			color[ci]
		);
	}
}

void Camera3D::DrawOtherCamera( // HACK
	const Camera3D& other) {
	// color = 0xfbbd0dFF
	// FrameAspect 16:9

	Matrix4x4 cameraMatrix = Matrix4x4::MakeAffine({ 1.0f, 1.0f, 1.0f }, other.camera_.rotate, other.camera_.translate);
	Vector3f worldCameraPos = CheckGraphicPipeline({ 0.0f }, cameraMatrix, viewMatrix_, projectionMatrix_, viewportMatrix_);

	//DrawSphere(other.camera_.translate, 0.02f, 16, 0xFAFAFA70); // debug

	// view point
	Vector3f viewPoint = {
		std::cos(-other.camera_.rotate.x) * std::sin(other.camera_.rotate.y),
		std::sin(-other.camera_.rotate.x),
		std::cos(-other.camera_.rotate.x) * std::cos(other.camera_.rotate.y),
	};


	viewPoint *= 0.3f;
	//DrawSphere(viewPoint, 0.02f, 16, 0xFAFAFAFF); // debug
	Matrix4x4 viewPointMatrix = Matrix4x4::MakeAffine(other.camera_.scale, other.camera_.rotate, viewPoint) * cameraMatrix;

	enum VertexName {
		LEFTTOP,
		RIGHTTOP,
		RIGHTBOTTOM,
		LEFTBOTTOM
	};

	// view frame
	Vector3f localVeretxPos[4];
	Vector3f worldVertexPos[4];

	localVeretxPos[LEFTTOP] = { -0.16f, 0.09f, 0.3f };
	localVeretxPos[RIGHTTOP] = { 0.16f, 0.09f, 0.3f };
	localVeretxPos[LEFTBOTTOM] = { -0.16f, -0.09f, 0.3f };
	localVeretxPos[RIGHTBOTTOM] = { 0.16f, -0.09f, 0.3f };

	for (int vi = 0; vi < 4; vi++) {
		worldVertexPos[vi] = CheckGraphicPipeline(localVeretxPos[vi], cameraMatrix, viewMatrix_, projectionMatrix_, viewportMatrix_);
	}

	DrawCoordinate(cameraMatrix, 0.2f);

	// frame draw
	for (int vi = 0; vi < 4; vi++) {
		Novice::DrawLine(
			static_cast<int>(worldVertexPos[vi].x),
			static_cast<int>(worldVertexPos[vi].y),
			static_cast<int>(worldVertexPos[(vi + 1) % 4].x),
			static_cast<int>(worldVertexPos[(vi + 1) % 4].y),
			0xfbbd0dFF
		);
	}

	// view draw
	for (int vi = 0; vi < 4; vi++) {
		Novice::DrawLine(
			static_cast<int>(worldVertexPos[vi].x),
			static_cast<int>(worldVertexPos[vi].y),
			static_cast<int>(worldCameraPos.x),
			static_cast<int>(worldCameraPos.y),
			0xfbbd0dFF
		);
	}
}

/* -----------------------------------------------------------------------------------------
 json
----------------------------------------------------------------------------------------- */
bool Camera3D::SaveCameraData(const std::string& cameraName) {

	// フォルダが無かったら作成
	std::filesystem::path dir(directory_);
	if (!std::filesystem::exists(dir)) {
		std::filesystem::create_directory(dir);
	}

	// jsonファイルパス作成
	std::string filePath = directory_ + cameraName + ".json";
	std::ofstream ofs;

	Json root;

	// Camera
	root["data"]["scale"] = {
		{"x", camera_.scale.x},
		{"y", camera_.scale.y},
		{"z", camera_.scale.z}
	};

	root["data"]["rotate"] = {
		{"x", camera_.rotate.x},
		{"y", camera_.rotate.y},
		{"z", camera_.rotate.z}
	};

	root["data"]["translate"] = {
		{"x", camera_.translate.x},
		{"y", camera_.translate.y},
		{"z", camera_.translate.z}
	};

	// blender
	root["blender"]["pos"] = {
		{"x", controlPoint_.x},
		{"y", controlPoint_.y},
		{"z", controlPoint_.z},
	};

	root["blender"]["rotate"] = {
		{"lon", lon_},
		{"lat", lat_},
		{"distance", distance_}
	};

	ofs.open(filePath);

	// load -> failed
	if (ofs.fail()) {
		assert(0);
		return false;
	}

	ofs << std::setw(4) << root << std::endl;
	ofs.close();

	return true;
}

bool Camera3D::LoadCameraData(const std::string& cameraName, LoadType type) {

	std::string filePath = directory_ + cameraName + ".json";
	std::ifstream ifs;
	Json root;

	ifs.open(filePath);

	if (ifs.fail()) {
		assert(false);
		return false;
	}

	ifs >> root;

	ifs.close();

	switch (type) {
		case LOAD_DATA:

			SetCamera(
				{ root["data"]["scale"]["x"], root["data"]["scale"]["y"], root["data"]["scale"]["z"] },
				{ root["data"]["rotate"]["x"], root["data"]["rotate"]["y"], root["data"]["rotate"]["z"] },
				{ root["data"]["translate"]["x"], root["data"]["translate"]["y"], root["data"]["translate"]["z"] }
			);

			break;

		case LOAD_BLENDER:

			SetCameraPoint(
				{ root["blender"]["pos"]["x"], root["blender"]["pos"]["y"], root["blender"]["pos"]["z"] },
				root["blender"]["rotate"]["lon"], root["blender"]["rotate"]["lat"], root["blender"]["rotate"]["distance"]
			);

			break;

		default:
			return false; // 想定外
			break;
	}

	// 正常終了
	return true;
}

bool Camera3D::SaveProjection(const std::string& cameraName) {
	// フォルダが無かったら作成
	std::filesystem::path dir(directory_);
	if (!std::filesystem::exists(dir)) {
		std::filesystem::create_directory(dir);
	}

	// jsonファイルパス作成
	std::string filePath = directory_ + cameraName + ".json";
	std::ofstream ofs;

	Json root;

	/*root["projection"][]*/

	return false;
}

void Camera3D::DebugCamera(
	const char* windowName, const char* cameraName) {

	ImGui::Begin(windowName);

	if (ImGui::TreeNode(cameraName)) {

		ImGui::Text("[scale]     x: %.4f, y: %.4f, z: %.4f", camera_.scale.x, camera_.scale.y, camera_.scale.z);
		ImGui::Text("[rotate]    x: %.4f, y: %.4f, z: %.4f", camera_.rotate.x, camera_.rotate.y, camera_.rotate.z);
		ImGui::Text("[translate] x: %.4f, y: %.4f, z: %.4f", camera_.translate.x, camera_.translate.y, camera_.translate.z);

		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::TreeNode("Detailed Infomation")) {

			ImGui::Text("[point] x: %.4f, y: %.4f, z: %.4f", controlPoint_.x, controlPoint_.y, controlPoint_.z);
			ImGui::Text("        lon: %.4f, lat: %.4f", lon_, lat_);
			ImGui::Text("        distance: %.4f", distance_);

			ImGui::TreePop();
		}

		ImGui::TreePop();
	}

	ImGui::End();
}



