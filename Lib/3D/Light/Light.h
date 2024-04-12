#pragma once
#include <ImGui.h>

// Lib > Geometry //
#include "MyVector3.h"

// Lib > Camera //
#include "Camera3D.h"

enum LightMode {
	kLightPoint,
	kLightSpot,
	kLightDirectional,
};

// various light setting
struct PointLight {
	float range;
};

struct SpotLight {
	float range;
	float angle;
	Vector3f direction;
};

struct DirectionalLight {
	Vector3f direction;
};

enum LightColor {
	COLOR_RED,
	COLOR_GREEN,
	COLOR_BLUE,
};

class Light {
public: // menber method //

	// constructor & destructor
	Light();
	~Light();

	// default method
	void Init();
	void Draw(const Camera3D& camera) const;

public: // menber object //

	LightMode lightMode_;

private: // menber object //

	Vector3f rotate_;
	Vector3f pos_;

	Matrix4x4 worldMatrix_;

	float intensity_;

	uint32_t alpha_;

	PointLight pointLight_;
	SpotLight spotLight_;
	DirectionalLight directionalLight_;

	// color
	float color_[3];

public: // user method //

	void UpdateImGui(const char* windowName, const char* lightName);

public: // accessor //

	void SetPos(const Vector3f& pos) { pos_ = pos; }

	Vector3f GetPos() const { return pos_; }
	float GetIntensity() const { return intensity_; }

	PointLight GetPointLightSetting() const { return pointLight_; }
	SpotLight GetSpotLightSetting() const { return spotLight_; }
	DirectionalLight GetDirectionalLightSetting() const { return directionalLight_; }

	void SetPointLight(const PointLight& pointLight) { pointLight_ = pointLight; }

	float GetColor(LightColor color) const { return color_[color]; }
	void SetColor(int r, int g, int b) {
		color_[COLOR_RED] = static_cast<float>(r) / 255.0f;
		color_[COLOR_GREEN] = static_cast<float>(g) / 255.0f;
		color_[COLOR_BLUE] = static_cast<float>(b) / 255.0f;
	}
};