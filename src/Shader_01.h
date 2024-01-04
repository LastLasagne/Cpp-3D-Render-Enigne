#pragma once
#ifndef Shader01_H
#define Shader01_H
#include "Shader.h"	

#endif 

class Shader_01: public Shader
{
	Vector3 pixelShader() override;
	virtual Vector3 vertexShader(Vector3* pos, Vector3* normal, Vector3 lightColor, float lightIntensity, Vector3 lighDir) override;

};

