#include "Graph.h"

namespace {
	float Clamp(float value, float min, float max) {
		if (value < min) {
			return min;

		} else if (value > max) {
			return max;

		} else {
			return value;
		}
	}
}

Graph::Graph() {
	Init();
}

Graph::~Graph() {
	ClearQueue();
}

void Graph::Init() {
}

void Graph::Update(float newValue) {
	AddQueue(newValue);
}

void Graph::Draw() {
	float dotValue = static_cast<float>(size_.y) / static_cast<float>(maxValues_);
	float dotTime = static_cast<float>(size_.x) / static_cast<float>((kMaxGraphSize_ - 2));

	for (int vi = 1; vi < kMaxGraphSize_; vi++) {
		if (values_.size() <= vi) { break; }

		float value = Clamp(values_[vi], 0.0f, maxValues_);
		float preValue = Clamp(values_[vi - 1], 0.0f, maxValues_);

		Vector2i pos = {
			pos_.x + (size_.x - vi * static_cast<int>(dotTime)),
			pos_.y + (size_.y - static_cast<int>(value * dotValue))
		};

		Vector2i prePos = {
			pos_.x + (size_.x - ((vi - 1) * static_cast<int>(dotTime))),
			pos_.y + (size_.y - static_cast<int>(preValue * dotValue))
		};

		/// -> size.y + pos.y は valueでいう0
		Novice::DrawLine(
			pos.x, pos.y,
			prePos.x, prePos.y,
			0x47d39dFF
		);
	}

	Novice::DrawBox(
		pos_.x, pos_.y,
		size_.x, size_.y,
		0.0f,
		0xFAFAFAFF,
		kFillModeWireFrame
	);
}

void Graph::ResetQueue() {
}

void Graph::ClearQueue() {
	values_.clear();
}

void Graph::AddQueue(float newValue) {
	// push
	values_.push_front(newValue);

	if (values_.size() > kMaxGraphSize_) {
		// pop
		values_.pop_back();
	}
}
