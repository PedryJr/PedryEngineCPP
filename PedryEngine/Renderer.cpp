#pragma once
#include "PedryEngine.h"

void Renderer::Initialize()
{
}

void Renderer::Simulate()
{
}

void Renderer::Update()
{

	GLuint batchIndex = transform->batchIndex;
	GLuint modelIndex = transform->modelIndex;

	DrawCallBatch* batches = transform->outerLocation->data();
	mat4* modelMatrices = batches[batchIndex].modelMatrices.data();

	batches[batchIndex].modelMatrices[modelIndex] = transform->GetModelMatrix();

}

Renderer::~Renderer()
{
}