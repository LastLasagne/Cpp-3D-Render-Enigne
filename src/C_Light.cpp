#include "C_Light.h"

C_Light::C_Light()
{
	name = Component::LIGHT;
	Color = Vector3(255, 0, 0);
	intensity = .75;
}
