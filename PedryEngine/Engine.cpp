#pragma once

#include "PedryEngine.h"

void Engine::Run()
{

	InitializeEngine();

	while (RenderSystem::ShouldTerminate())
	{

		UpdateLoop();

	}
}

void Engine::InitializeEngine()
{
	//Engine::objectIterator.setFunction([](GameObject* obj, GLint index) { obj->Simulate(); });
	glfwInit();

	timer = TimeNow;
	
	RenderSystem::Initialize();
	RenderSystem::OpenGLEnableCaps();

	game = new Game();
	game->Start();
}

void Engine::UpdateLoop()
{
	TimelineUpdate();
	InputUpdate();
	GameUpdate();
	GraphicsUpdate();

}

void Engine::InputUpdate()
{
		glfwPollEvents();
		GlobalCamera::UpdateInput(deltaTime);
}

void Engine::TimelineUpdate()
{
	Clock newTimer = TimeNow;
	deltaTime = Duration(timer, newTimer);
	timer = newTimer;
	timeAlive += deltaTime;
}

void Engine::GraphicsUpdate()
{
	DrawCallManager::ExecuteDrawCalls();
	RenderSystem::SwapBuffer();
}

void Engine::GameUpdate()
{
	GameObject::UpdateAll();
	game->Update();
}

GLdouble Engine::deltaTime = 0.0;
GLdouble Engine::timeAlive = 0.0;

vec3 Engine::lightPos = vec3(0.0f, 3.0f, 0.0f);
GLfloat Engine::lightFarPlane = 400.0f;
mat4 Engine::lightProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, lightFarPlane);

ParallelIterator<GameObject*> Engine::objectIterator;