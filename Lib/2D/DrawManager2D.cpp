#include "DrawManager2D.h"

DrawManager2D* DrawManager2D::GetInstance() {
	static DrawManager2D instance;
	return &instance;
}

void DrawManager2D::AddElement(LayerDrawElement element) {
	
	if (element.layer == NULL) {
		element.layer = kMaxLayer_ - elementNum_;
	}

	elements_.push_back(element);
	elementNum_++;
}

void DrawManager2D::Draw() {
	std::sort(elements_.begin(), elements_.end(), [](const auto& a, const auto& b) {
		// layerが大きいほど先に描画
		return a.layer > b.layer;
	});

	

	ClearElementStack();
}

void DrawManager2D::ClearElementStack() {
	elements_.clear();
	elementNum_ = 0;
}

DrawManager2D::DrawManager2D() {
	Init();
}

void DrawManager2D::Init() {
}

void DrawManager2D::DrawElements(const DrawElement& element) {
	Novice::SetBlendMode(element.blendMode);

	switch (element.type) {
		case kTriangle:
			Novice::DrawTriangle(
				static_cast<int>(element.pos[0].x), static_cast<int>(element.pos[0].y),
				static_cast<int>(element.pos[1].x), static_cast<int>(element.pos[1].y),
				static_cast<int>(element.pos[2].x), static_cast<int>(element.pos[2].y),
				element.color,
				element.fillMode
			);
			break;

		case kBox:
			Novice::DrawBox(
				static_cast<int>(element.pos[0].x), static_cast<int>(element.pos[0].y),
				static_cast<int>(element.size.x), static_cast<int>(element.size.y),
				element.angle,
				element.color,
				element.fillMode
			);
			break;

		case kEllipse:
			Novice::DrawEllipse(
				static_cast<int>(element.pos[0].x), static_cast<int>(element.pos[0].y),
				static_cast<int>(element.size.x), static_cast<int>(element.size.y),
				element.angle,
				element.color,
				element.fillMode
			);
			break;

		case kQuad:
			Novice::DrawQuad(
				static_cast<int>(element.pos[0].x), static_cast<int>(element.pos[0].y),
				static_cast<int>(element.pos[1].x), static_cast<int>(element.pos[1].y),
				static_cast<int>(element.pos[2].x), static_cast<int>(element.pos[2].y),
				static_cast<int>(element.pos[3].x), static_cast<int>(element.pos[3].y),
				element.textureRectPos.x, element.textureRectPos.y,
				element.textureRectSize.x, element.textureRectSize.y,
				element.texture,
				element.color
			);
			break;

		case kSprite:
			Novice::DrawSprite(
				static_cast<int>(element.pos[0].x), static_cast<int>(element.pos[0].y),
				element.texture,
				element.size.x / static_cast<float>(element.textureFullSize.x),
				element.size.y / static_cast<float>(element.textureFullSize.y),
				element.angle,
				element.color
			);
			break;

		case kSpriteRect:
			Novice::DrawSpriteRect(
				static_cast<int>(element.pos[0].x), static_cast<int>(element.pos[0].y),
				element.textureRectPos.x, element.textureRectPos.y,
				element.textureRectSize.x, element.textureRectSize.y,
				element.texture,
				element.size.x / static_cast<float>(element.textureFullSize.x),
				element.size.y / static_cast<float>(element.textureFullSize.y),
				element.angle,
				element.color
			);
			break;

		default:
			// 想定外
			break;
	}

	Novice::SetBlendMode(kBlendModeNormal);
}
