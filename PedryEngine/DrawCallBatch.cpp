#pragma once
#include "PedryEngine.h"
class Transform;
DrawCallBatch::DrawCallBatch(Shader* shader, Mesh* mesh)
{
	this->shader = shader;
	this->mesh = mesh;

	std::cout << "BATCH CREATED: " << this << std::endl;

}

DrawCallBatch::~DrawCallBatch()
{
}

void DrawCallBatch::AddBatchInstance(Transform* instanceSource)
{

	std::cout << "BATCH ASSIGNED: " << this << std::endl;

	mat4 newMatrix = mat4(0.0f);

	modelMatrices.push_back(newMatrix);

	instanceSource->SetModelLocation(*this, modelMatrices.size() - 1);
	std::cout << "NEW MATRIX INSTANCE ADRESS: " << &modelMatrices.data()[modelMatrices.size() - 1] << std::endl;

}
