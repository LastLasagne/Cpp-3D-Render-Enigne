#include "Vector2.h"

Vector2::Vector2()
{
	x = 0;
	y = 0;
}

Vector2::Vector2(float newX, float newY)
{
	x = newX;
	y = newY;
}

void Vector2::addVector(Vector2 v)
{
	x += v.x;
	x += v.y;
}
