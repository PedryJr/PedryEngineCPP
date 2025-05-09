#pragma once
#include "PedryEngine.h"

Vector<DrawCallBatch> DrawCallManager::drawCalls;

void DrawCallManager::ExecuteDrawCalls()
{
	for (DrawCallBatch& batch : drawCalls)
	{

		batch.shader->EnsureUseProgram(batch.shader->GetProgramID());
		Renderer::DrawBatch(&batch);
	}
}

void DrawCallManager::AddDrawCall(Mesh* mesh, Shader* shader, Transform* transform)
{

	for (DrawCallBatch& batch : drawCalls)
	{

		if (!(batch.mesh->meshId == mesh->meshId)) continue;
		batch.AddBatchInstance(transform);
		return;

	}

	drawCalls.emplace_back(shader, mesh);
	drawCalls.back().AddBatchInstance(transform);


}
