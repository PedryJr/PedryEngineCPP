#pragma once
#include "PedryEngine.h"
#define mainTextLocation GetGameObject().GetComponent<Transform>().GetOuterLocation().data()[GetGameObject().GetComponent<Transform>().batchIndex].gameSwizzle->modelMainTextures[GetGameObject().GetComponent<Transform>().modelIndex]

void Renderer::Initialize()
{
	

	if (!initialized)
	{
		shader = AssetManager::LoadShader("default");
		sphere = AssetManager::LoadMesh("Gonagon");
	}
	DrawCallManager::AddDrawCall(sphere, shader, GetGameObject().GetComponent<Transform>());
	initialized = true;
}

void Renderer::Simulate()
{
}

void Renderer::Update()
{

}

void Renderer::Demolish()
{
	DrawCallManager::RemoveDrawCall(sphere, shader, GetGameObject().GetComponent<Transform>());
	GetGameObject().GetComponent<Transform>().modelIndex = -1;

}

void Renderer::ChangeMesh(Mesh* newMesh)
{
	Transform& transform = GetGameObject().GetComponent<Transform>();
	DrawCallManager::RemoveDrawCall(sphere, shader, transform);
	sphere = newMesh;
	DrawCallManager::AddDrawCall(newMesh, shader, transform);
}

void Renderer::ChangeTexture(GLuint64 texture)
{
	mainTextLocation = texture;
}

GENERATED_ACTION(Renderer)