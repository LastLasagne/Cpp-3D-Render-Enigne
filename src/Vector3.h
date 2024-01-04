#pragma once
class Vector3
{
public:
	float x;
	float y;
	float z;

	Vector3();

	Vector3(float newX, float newY, float newZ);

	//functions used in render piepeline
	Vector3 addVector(Vector3 v);
	Vector3 multiplyFloat(float f);	
};

