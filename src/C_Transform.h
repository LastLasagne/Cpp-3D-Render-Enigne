#pragma once

#ifndef Transform_H
#define Transform_H

#include "Component.h"
#include "Vector3.h"
#endif

class C_Transform: public Component
{
public:
	C_Transform();

	//getter for transforms
	Vector3 getPosition(){return position;}
	Vector3 getRotation() { return rotation; }
	Vector3 getScale() { return scale; }

	//setter for transforms
	void setPosition(Vector3 p) { position = p; }
	void setRotation(Vector3 r) { rotation = r; }
	void setScale(Vector3 s) { scale = s; }

	//getter for reverse matrizes
	Vector3 getReversePosition();
	Vector3 getReverseRotation();
	Vector3 getReverseScale();

	void doSomething() override {}

private:
	//transform channels
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
};

