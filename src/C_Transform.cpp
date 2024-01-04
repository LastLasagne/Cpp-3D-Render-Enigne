#include "C_Transform.h"

C_Transform::C_Transform()
{
	position = Vector3(0, 0, 0);
	rotation = Vector3(0, 0, 0);
	scale = Vector3(1, 1, 1);

	name = Component::TRANSFORM;
}

Vector3 C_Transform::getReversePosition() {
	Vector3 reverse;
	reverse.x = -position.x;
	reverse.y = -position.y;
	reverse.z = -position.z;
	return reverse;
}

Vector3 C_Transform::getReverseRotation() {
	Vector3 reverse;
	reverse.x = -rotation.x;
	reverse.y = -rotation.y;
	reverse.z = -rotation.z;
	return reverse;
}

Vector3 C_Transform::getReverseScale() {
	Vector3 reverse;
	reverse.x = 1 / scale.x;
	reverse.y = 1 / scale.y;
	reverse.z = 1 / scale.z;
	return reverse;
}