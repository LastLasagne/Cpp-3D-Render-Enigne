#pragma once

#ifndef MeshRenderer_H
#define MeshRenderer_H

#include "Vertex.h"
#include <vector>
#include "Component.h"
#include "Shader.h"

#endif

class C_MeshRenderer:public Component
{
public:
	C_MeshRenderer(std::vector<Vertex*> vBuffer, std::vector<int> vIBuffer, std::vector<Vector3*> nBuffer);

	//getter functions for buffer
	std::vector<Vertex*> getVertexBuffer();
	std::vector<int> getIndexBuffer();
	std::vector<Vector3*> getNormalBuffer();

	//used to store a copy of the buffer from a certain time so it can get used later unchanged
	void resetTempVertexBuffer();
	void resetTempNormalBuffer();

	bool clipped = false;
	Shader* shader;

private:
	//buffer
	std::vector<Vertex*> vertexBuffer;
	std::vector<Vertex*> tempVertexBuffer;

	std::vector<int> vertexIndexBuffer;

	std::vector<Vector3*> normalBuffer;
	std::vector<Vector3*> tempNormalBuffer;

	void doSomething() override {}
};

