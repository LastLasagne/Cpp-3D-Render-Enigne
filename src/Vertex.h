#pragma once

#ifndef Vertex_H
#define Vertex_H

#include "Vector3.h"
#endif

class Vertex		//vertex holding position and color
{
public:
	Vertex(Vector3 newPosition);

	Vector3* getPosition();

	void setPosition(Vector3 p);

	Vector3 Color;
private:
	Vector3 position;
};

