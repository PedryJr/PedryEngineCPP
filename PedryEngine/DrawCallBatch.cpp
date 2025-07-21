#pragma once
#include "PedryEngine.h"
DrawCallBatch::DrawCallBatch(Shader* shader, Mesh* mesh)
{

	renderSwizzle = new Swizzle();
	gameSwizzle = new Swizzle();

	this->notifyLayoutChange = false;
	this->shader = shader;
	this->mesh = mesh;


}

DrawCallBatch::~DrawCallBatch()
{
}

int lol = 0;

void DrawCallBatch::AddBatchInstance(Transform& instanceSource, GLint batchIndex)
{
	GLuint64 newTextureID;

	if (lol % 2 == 0) newTextureID = AssetManager::LoadTexture("default");
	else newTextureID = AssetManager::LoadTexture("default");
	newTextureID = AssetManager::LoadTexture("default");
	lol++;

	mat4 newMatrix = mat4(1.0f);

	gameSwizzle->corespondingTransforms.push_back(&instanceSource);
	gameSwizzle->modelMatrices.push_back(newMatrix);
	gameSwizzle->modelMainTextures.push_back(newTextureID);
	gameSwizzle->modelHeightTextures.push_back(newTextureID);

	instanceSource.SetModelLocation(gameSwizzle->modelMatrices.size() - 1, batchIndex);

}