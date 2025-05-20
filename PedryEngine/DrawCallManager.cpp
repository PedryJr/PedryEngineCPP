#pragma once
#include "PedryEngine.h"

Vector<DrawCallBatch> DrawCallManager::drawCalls;

void DrawCallManager::ExecuteDrawCalls()
{

	RenderSystem::PrepareShadowPass();
	for (DrawCallBatch& batch : drawCalls) RenderSystem::DrawBatchShadow(&batch);

	RenderSystem::PrepareNormalPass();
	for (DrawCallBatch& batch : drawCalls) RenderSystem::DrawBatchNormal(&batch);

}

void DrawCallManager::AddDrawCall(Mesh* mesh, Shader* shader, Transform* transform)
{

	transform->outerLocation = &drawCalls;

	for (GLint batchIndex = 0; batchIndex < drawCalls.size(); batchIndex++) 
	{

		DrawCallBatch& batch = drawCalls.at(batchIndex);

		if ((batch.mesh->meshId == mesh->meshId) && (batch.shader->shaderId == shader->shaderId))
		{
			batch.AddBatchInstance(*transform, batchIndex);
			return;
		}
	}

	drawCalls.emplace_back(shader, mesh);
	drawCalls.back().AddBatchInstance(*transform, drawCalls.size() - 1);

}

void DrawCallManager::RemoveDrawCall(Mesh* mesh, Shader* shader, Transform* transform)
{
	
	DrawCallBatch* batchToEraseFrom = nullptr;
	for (DrawCallBatch& batch : drawCalls) 
	{
		if (batch.mesh->meshId != mesh->meshId) continue;
		batchToEraseFrom = &batch;
		break;
	}

	if (batchToEraseFrom == nullptr) return;

	Vector<Transform>& transformCollectionToEraseFrom = batchToEraseFrom->corespondingTransforms;
	Vector<mat4>& modelColletionToEraseFrom = batchToEraseFrom->modelMatrices;

	Transform endBuffer = transformCollectionToEraseFrom.at(transformCollectionToEraseFrom.size() - 1);

	transformCollectionToEraseFrom.at(transformCollectionToEraseFrom.size() - 1) = transformCollectionToEraseFrom.at(transform->modelIndex);
	transformCollectionToEraseFrom.at(transform->modelIndex) = endBuffer;
	
	modelColletionToEraseFrom.at(transform->modelIndex) = modelColletionToEraseFrom.at(modelColletionToEraseFrom.size() - 1);

	transformCollectionToEraseFrom.resize(transformCollectionToEraseFrom.size() - 1);
	modelColletionToEraseFrom.resize(modelColletionToEraseFrom.size() - 1);

}
