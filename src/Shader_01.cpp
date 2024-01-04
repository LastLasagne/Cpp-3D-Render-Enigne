#include "Shader_01.h"

//pixelShader just returning a flat color
Vector3 Shader_01::pixelShader()
{
	return Vector3(0, 255, 0);
}

//vertex shader with dot product light calculation
Vector3 Shader_01::vertexShader(Vector3* pos, Vector3* normal, Vector3 lightColor, float lightIntensity, Vector3 lightDir)
{
	float dot = normal->x * -lightDir.x + normal->y * -lightDir.z + normal->z * -lightDir.y; //error
	
	if (dot > 0.0)
	{
		Vector3 diffRefl = lightColor.multiplyFloat(dot).multiplyFloat(lightIntensity);
		return diffRefl;
	}
	else return Vector3(0,0,0);
}

