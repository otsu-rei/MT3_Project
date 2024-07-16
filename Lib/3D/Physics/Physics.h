#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
// geometry
#include <MyVector3.h>

// c++
#include <cstdint>

// imgui
#include <imgui.h>

////////////////////////////////////////////////////////////////////////////////////////////
// variables
////////////////////////////////////////////////////////////////////////////////////////////

static float deltaTime = 1.0f / 60.0f;
// todo: 可変フレームレートに対応

static const float kGrabity = 9.8f;

////////////////////////////////////////////////////////////////////////////////////////////
// structure
////////////////////////////////////////////////////////////////////////////////////////////

struct Ball {
	Vector3f position;     //!< 位置
	Vector3f velocity;     //!< 速度
	Vector3f acceleration; //!< 加速度
	
	float mass;     //!< 質量
	float radius;   //!< 半径
	uint32_t color; //!< 色

	void SetImGui() {
		ImGui::DragFloat3("position",     &position.x,     0.02f);
		ImGui::DragFloat3("velocity",     &velocity.x,     0.02f);
		ImGui::DragFloat3("acceleration", &acceleration.x, 0.02f);

		ImGui::DragFloat("mass", &mass, 0.02f);
	}
};

struct Spring {
	Vector3f anchor;          //!< 固定された位置
	float natureLength;       //!< 自然長
	float stiffness;          //!< 剛性. ばね定数"k"
	float dampingCoefficient; //!< 減衰係数

	void SetImGui() {
		ImGui::DragFloat3("position",          &anchor.x,     0.02f);
		ImGui::DragFloat("natureLength",       &natureLength, 0.02f);
		ImGui::DragFloat("stiffness",          &stiffness,    0.02f);
		ImGui::DragFloat("dampingCoefficient", &dampingCoefficient, 0.02f);
	}
};

struct Pendulum {
	Vector3f anchor;           //!< 固定された位置
	float length;              //!< 紐の長さ
	float angle;               //!< 現在の角度
	float angularVelocity;     //!< 角速度
	float angularAcceleration; //!< 角加速度

	void SetImGui() {
		ImGui::DragFloat3("anchor",             &anchor.x, 0.02f);
		ImGui::DragFloat("length",              &length, 0.02f);
		ImGui::DragFloat("angle",               &angle, 0.02f);
		ImGui::DragFloat("angleVelocity",       &angularVelocity, 0.02f);
		ImGui::DragFloat("angularAcceleration", &angularAcceleration, 0.02f);
	}
};
