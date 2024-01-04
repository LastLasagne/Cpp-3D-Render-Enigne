#pragma once

#ifndef Light_H
#define Light_H
#define _USE_MATH_DEFINES

#include "Component.h"
#include "Vector3.h"

#endif

class C_Light : public Component
{
public:
	C_Light();

	//light specific values
	Vector3 Color;
	float intensity;
	Vector3 lightDir;

	void doSomething() override {}
};

