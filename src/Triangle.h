#pragma once

#ifndef Triangle_H
#define Triangle_H

#include <vector>
#include "Vector3.h"
#include "C_MeshRenderer.h"

#endif

class Triangle		//triangle class holding vertex positions, normal and color
{
public:
	Triangle();
	std::vector<Vector3*> pos = {new Vector3, new Vector3, new Vector3 };
	Vector3 normal;
	C_MeshRenderer* meshRenderer;

	std::vector<Vector3> col = { Vector3(0,0,0), Vector3(0,0,0), Vector3(0,0,0) };
};

