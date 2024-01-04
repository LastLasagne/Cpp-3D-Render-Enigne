#include "C_Camera.h"

C_Camera::C_Camera()
{ 
	name = Component::CAMERA; 
}

//fetch the projection matrix
std::vector<std::vector<float>> C_Camera::projectionMatrix()
{
	//mode of projection perspective or orthographic
	if (perspective)
	{
		float aspectRatio = res[0] / res[1];

		float f = 1.0f / tan(0.5f * fovY * M_PI / 180.0f);

		std::vector<std::vector<float>> result = 
		
		{
			{f / aspectRatio, 0.0f, 0.0f, 0.0f},
			{0.0f, f, 0.0f, 0.0f},
			{0.0f, 0.0f, (farClip + nearClip) / (nearClip - farClip), (2.0f * farClip * nearClip) / (nearClip - farClip)},
			{0.0f, 0.0f, -1.0f, 0.0f}
		};
		return result;
	}
	else
	{
		std::vector<std::vector<float>> result
		{
			{2.0f / res[0], 0,0,0},
			{0, 2.0f / res[1],0,0},
			{0,0,-2 / (nearClip - farClip), (nearClip + farClip) / farClip - nearClip},
			{0,0,0,1}
		};
		return result;
	}
}