#pragma once
#include "PedryEngine.h"

Vector<DrawCallBatch> DrawCallManager::drawCalls;

void DrawCallManager::ExecuteDrawCalls()
{

	//glFrontFace(GL_CW);
	//Shadow mapping pass
	for (DrawCallBatch& batch : drawCalls)
	{
		batch.shader->EnsureUseProgram(batch.shader->GetShadowID(), batch.shader->GetVertexArrayID());
		RenderSystem::DrawBatchShadow(&batch);
	}

	//glFrontFace(GL_CCW);
	//Normal pass
	for (DrawCallBatch& batch : drawCalls)
	{
		batch.shader->EnsureUseProgram(batch.shader->GetProgramID(), batch.shader->GetVertexArrayID());
		RenderSystem::DrawBatchNormal(&batch);
	}

}

void DrawCallManager::AddDrawCall(Mesh* mesh, Shader* shader, Transform* transform)
{

	for (DrawCallBatch& batch : drawCalls)
	{

		if (!(batch.mesh->meshId == mesh->meshId)) continue;
		batch.AddBatchInstance(*transform);
		return;

	}

	drawCalls.emplace_back(shader, mesh);
	drawCalls.back().AddBatchInstance(*transform);


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
