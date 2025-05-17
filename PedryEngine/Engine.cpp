#pragma once

#include "PedryEngine.h"

void Engine::Run()
{
	std::this_thread::sleep_for(std::chrono::seconds(2));

	InitializeEngine();

	while (RenderSystem::ShouldTerminate())
	{

		UpdateLoop();

	}
}

void Engine::InitializeEngine()
{
	//Parallel::Init(16);
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
	game->Update();
	//Parallel::Wait();
}

GLdouble Engine::deltaTime = 0.0;
GLdouble Engine::timeAlive = 0.0;

vec3 Engine::lightPos = vec3(0.0f, 3.0f, 0.0f);
GLfloat Engine::lightFarPlane = 2000.0f;
mat4 Engine::lightProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, lightFarPlane);

ParallelIterator<vec4> Engine::vecIterator;