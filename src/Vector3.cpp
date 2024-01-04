#include "Vector3.h"

Vector3::Vector3()
{
	x = 0;
	y = 0;
	z = 0;
}

Vector3::Vector3(float newX, float newY, float newZ)
{
	x = newX;
	y = newY;
	z = newZ;
}

Vector3 Vector3::addVector(Vector3 v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3 Vector3::multiplyFloat(float f)
{
	x *= f;
	y *= f;
	z *= f;
	return *this;
}