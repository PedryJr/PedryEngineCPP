#pragma once
#include "PedryEngine.h"
Vector<DrawCallBatch> DrawCallManager::drawCalls;

void DrawCallManager::ExecuteDrawCalls()
{

	GLuint size = drawCalls.size();
	DrawCallBatch* ptr = drawCalls.data();

	RenderSystem::PrepareShadowPass();
	for (GLuint index = 0; index < size; index++) RenderSystem::DrawBatchShadow(&ptr[index]);
	RenderSystem::PrepareNormalPass();
	for (GLuint index = 0; index < size; index++) RenderSystem::DrawBatchNormal(&ptr[index]);
}

void DrawCallManager::AddDrawCall(Mesh* mesh, Shader* shader, Transform& transform)
{


	transform.useContigousArray = true;

	for (GLint batchIndex = 0; batchIndex < drawCalls.size(); batchIndex++) 
	{

		DrawCallBatch& batch = drawCalls.at(batchIndex);

		if ((batch.mesh->meshAssetId == mesh->meshAssetId) && (batch.shader->shaderAssetId == shader->shaderAssetId))
		{
			batch.AddBatchInstance(transform, batchIndex);
			return;
		}
	}

	drawCalls.emplace_back(shader, mesh);
	drawCalls.back().AddBatchInstance(transform, drawCalls.size() - 1);
}
void DrawCallManager::RemoveDrawCall(Mesh* mesh, Shader* shader, Transform& transform)
{
	
	DrawCallBatch* batchToEraseFrom = nullptr;
	for (DrawCallBatch& batch : drawCalls) 
	{
		if ((batch.mesh->meshAssetId != mesh->meshAssetId) || (batch.shader->shaderAssetId != shader->shaderAssetId)) continue;
		batchToEraseFrom = &batch;
		break;
	}

	if (batchToEraseFrom == nullptr) return;


	Vector<Transform*>& transformsToEraseFrom = batchToEraseFrom->gameSwizzle->corespondingTransforms;
	Vector<mat4>& modelsToEraseFrom = batchToEraseFrom->gameSwizzle->modelMatrices;
	Vector<GLuint64>& modelMTToRemoveFrom = batchToEraseFrom->gameSwizzle->modelMainTextures;
	Vector<GLuint64>& modelHTToRemoveFrom = batchToEraseFrom->gameSwizzle->modelHeightTextures;


	GLuint indexToErase = transform.modelIndex;
	GLuint lastIndex = transformsToEraseFrom.size() - 1;

	transform.useContigousArray = false;

	transformsToEraseFrom.at(indexToErase) = transformsToEraseFrom.at(lastIndex);
	modelsToEraseFrom.at(indexToErase) = modelsToEraseFrom.at(lastIndex);
	modelMTToRemoveFrom.at(indexToErase) = modelMTToRemoveFrom.at(lastIndex);
	modelHTToRemoveFrom.at(indexToErase) = modelHTToRemoveFrom.at(lastIndex);

	transformsToEraseFrom.resize(transformsToEraseFrom.size() - 1);
	modelsToEraseFrom.resize(modelsToEraseFrom.size() - 1);
	modelMTToRemoveFrom.resize(modelMTToRemoveFrom.size() - 1);
	modelHTToRemoveFrom.resize(modelHTToRemoveFrom.size() - 1);

	if(indexToErase != lastIndex) batchToEraseFrom->gameSwizzle->corespondingTransforms[indexToErase]->modelIndex = indexToErase;

}