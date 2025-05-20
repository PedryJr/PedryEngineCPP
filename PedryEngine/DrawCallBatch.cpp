#pragma once
#include "PedryEngine.h"
DrawCallBatch::DrawCallBatch(Shader* shader, Mesh* mesh)
{
	this->notifyLayoutChange = false;
	this->shader = shader;
	this->mesh = mesh;

	std::cout << "BATCH CREATED: " << this << std::endl;

}

DrawCallBatch::~DrawCallBatch()
{
}
int lol;
void DrawCallBatch::AddBatchInstance(Transform& instanceSource, GLint batchIndex)
{

	std::cout << "BATCH ASSIGNED: " << this << std::endl;


	mat4 newMatrix = mat4(0.0f);
	GLuint64 newTextureID = AssetManager::LoadTexture("Marble");

	lol++;

	corespondingTransforms.push_back(instanceSource);
	modelMatrices.push_back(newMatrix);
	modelMainTextures.push_back(newTextureID);
	modelHeightTextures.push_back(AssetManager::LoadTexture("MarbleHeight"));

	instanceSource.SetModelLocation(modelMatrices.size() - 1, batchIndex);
	std::cout << "NEW MATRIX INSTANCE ADRESS: " << &modelMatrices.data()[modelMatrices.size() - 1] << std::endl;

}
