#pragma once

#ifndef Camera_H
#define Camera_H
#define _USE_MATH_DEFINES

#include "Component.h"
#include <vector>
#include <cmath>
#include "Vector3.h"

#endif

class C_Camera: public Component
{
public:
	C_Camera();

	//camera specific values
	bool perspective = true;
	float res[2] = {1, 1 };
	float nearClip = .0f;
	float farClip = 1.0f;
	float fovY = 60.0f; 

	Vector3 camDir;

	void adjustResolution(float x, float y)
	{
		res[1] = y / x;
	}

	void doSomething() override {}

	std::vector<std::vector<float>> projectionMatrix();
};

