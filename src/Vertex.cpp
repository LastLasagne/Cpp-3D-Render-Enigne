#include "Vertex.h"

Vertex::Vertex(Vector3 newPosition)
{
	position = newPosition;
}

Vector3* Vertex::getPosition()
{
	return &position;
}

void Vertex::setPosition(Vector3 p)
{
	position = p;
}