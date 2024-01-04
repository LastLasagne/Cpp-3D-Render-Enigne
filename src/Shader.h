#pragma once

#ifndef Shader_H
#define Shader_H
#include "Vector3.h"
#endif

class Shader		//shader wrapper class holding a vertexShader and pixelShader
{
public:
	virtual ~Shader() {}

	virtual Vector3 vertexShader(Vector3* pos, Vector3* normal, Vector3 lightColor, float lightIntensity, Vector3 lighDir) { return Vector3(0, 0, 0); }

	virtual Vector3 pixelShader() { return Vector3(0,0,0); };
};