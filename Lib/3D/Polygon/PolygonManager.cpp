#include "PolygonManager.h"

namespace {
	bool SortLayer(const SortPolygonElement& a, const SortPolygonElement& b) {
		return a.layer > b.layer;
	}

	bool SortDistance(const SortPolygonElement& a, const SortPolygonElement& b) {
		if (a.layer == b.layer) {
			return a.distance > b.distance;

		} else {
			return false;
		}
	}

	void DrawQuadSubdivision(
		const Vector3f& leftTop, const Vector3f& rightTop, const Vector3f& leftBottom, const Vector3f& rightBottom,
		uint32_t kSubdivision, int texture, const Vector2f& textureSize, uint32_t color) {

		Vector2f topEvery = { rightTop.x - leftTop.x, rightTop.y - leftTop.y, };
		topEvery = {
			topEvery.x / kSubdivision,
			topEvery.y / kSubdivision,
		};

		Vector2f bottomEvery = { rightBottom.x - leftBottom.x, rightBottom.y - leftBottom.y };
		bottomEvery = {
			bottomEvery.x / kSubdivision,
			bottomEvery.y / kSubdivision,
		};

		Vector2f textureEvery = {
			textureSize.x / kSubdivision,
			textureSize.y,
		};

		for (uint32_t xIndex = 0; xIndex < kSubdivision; xIndex++) {
			Novice::DrawQuad(
				static_cast<int>(leftTop.x + topEvery.x * xIndex),
				static_cast<int>(leftTop.y + topEvery.y * xIndex),
				static_cast<int>(leftTop.x + topEvery.x * (xIndex + 1)),
				static_cast<int>(leftTop.y + topEvery.y * (xIndex + 1)),
				static_cast<int>(leftBottom.x + bottomEvery.x * xIndex),
				static_cast<int>(leftBottom.y + bottomEvery.y * xIndex),
				static_cast<int>(leftBottom.x + bottomEvery.x * (xIndex + 1)),
				static_cast<int>(leftBottom.y + bottomEvery.y * (xIndex + 1)),
				static_cast<int>(textureEvery.x * xIndex),
				0,
				static_cast<int>(textureEvery.x),
				static_cast<int>(textureEvery.y),
				texture,
				color
			);
		}
	}
}

PolygonManager::PolygonManager() {
}

void PolygonManager::SortPolygon() {
	// layer sort
	std::sort(polygons_.begin(), polygons_.end(), SortLayer);

	// distance sort
	std::stable_sort(polygons_.begin(), polygons_.end(), SortDistance);
}

void PolygonManager::DrawPolygonElement(const PolygonElement& element) {
	Novice::SetBlendMode(element.blendMode);

	switch (element.polygonType) {
		case PolygonType::kPolygonTriangle:

			Novice::DrawTriangle(
				static_cast<int>(element.screenPos[A].x), static_cast<int>(element.screenPos[A].y),
				static_cast<int>(element.screenPos[B].x), static_cast<int>(element.screenPos[B].y),
				static_cast<int>(element.screenPos[C].x), static_cast<int>(element.screenPos[C].y),
				element.color, element.fillMode
			);

			break;

		case PolygonType::kPolygonQuad:

			if (element.fillMode == kFillModeSolid) {
				Novice::DrawQuad(
					static_cast<int>(element.screenPos[A].x), static_cast<int>(element.screenPos[A].y),
					static_cast<int>(element.screenPos[B].x), static_cast<int>(element.screenPos[B].y),
					static_cast<int>(element.screenPos[D].x), static_cast<int>(element.screenPos[D].y),
					static_cast<int>(element.screenPos[C].x), static_cast<int>(element.screenPos[C].y),
					0, 0,
					1, 1,
					GameTextureManager::GetHandle("white1x1"),
					element.color
				);

			} else {
				for (int vi = 0; vi < 4; vi++) {
					Novice::DrawLine(
						static_cast<int>(element.screenPos[vi].x), static_cast<int>(element.screenPos[vi].y),
						static_cast<int>(element.screenPos[(vi + 1) % 4].x), static_cast<int>(element.screenPos[(vi + 1) % 4].y),
						element.color
					);
				}
			}

			break;

		case PolygonType::kPolygonQuadTexture:

			DrawQuadSubdivision(
				element.screenPos[A],
				element.screenPos[B],
				element.screenPos[D],
				element.screenPos[C],
				element.subdivision,
				element.texture,
				element.textureSize,
				element.color
			);

			break;

		default:
			assert(false); // 想定外のType
			break;
	}

	Novice::SetBlendMode(kBlendModeNormal);
}

PolygonManager* PolygonManager::GetInstance() {
	static PolygonManager instance;
	return &instance;
}

void PolygonManager::AddPolygonElement(const SortPolygonElement& element) {
	polygons_.push_back(element);
}

void PolygonManager::DrawPolygon() {

	// Polygonsをソート
	SortPolygon();

	for (const auto& it : polygons_) {
		DrawPolygonElement(it);
	}

	ClearPolygonStack();
}

void PolygonManager::ClearPolygonStack() {
	polygons_.clear();
}

void PolygonManager::ImGuiDebug() {
	ImGui::Begin("PolygonManager.h");
	int number = 0;

	for (const auto& polygon : polygons_) {
		ImGui::Text("Number %d : layer %d, distace %f", number, polygon.layer, polygon.distance);
		number++;
	}
	ImGui::End();
}
