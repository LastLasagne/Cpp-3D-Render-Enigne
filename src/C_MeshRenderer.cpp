#include "C_MeshRenderer.h"

C_MeshRenderer::C_MeshRenderer(std::vector<Vertex*> vBuffer, std::vector<int> vIBuffer, std::vector<Vector3*> nBuffer)
{
	vertexBuffer = vBuffer;
	vertexIndexBuffer = vIBuffer;
	normalBuffer = nBuffer;
	tempVertexBuffer.resize(vertexBuffer.size(), nullptr);
	tempNormalBuffer.resize(normalBuffer.size(), nullptr);
	name = Component::MESH_RENDERER;
}

std::vector<Vertex*> C_MeshRenderer::getVertexBuffer()
{
	return tempVertexBuffer;
}

std::vector<int> C_MeshRenderer::getIndexBuffer()
{
	return vertexIndexBuffer;
}

std::vector<Vector3*> C_MeshRenderer::getNormalBuffer()
{
	return tempNormalBuffer;
}

void C_MeshRenderer::resetTempVertexBuffer()
{
	for (int i = 0; i < vertexBuffer.size(); ++i) {
		// copy the vertices
		tempVertexBuffer[i] = new Vertex(*vertexBuffer[i]);
	}
}

void C_MeshRenderer::resetTempNormalBuffer()
{
	for (int i = 0; i < normalBuffer.size(); ++i) {
		// copy the vertices
		tempNormalBuffer[i] = new Vector3(*normalBuffer[i]);
	}
}