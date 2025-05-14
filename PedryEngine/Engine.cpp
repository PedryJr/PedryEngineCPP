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
}

void Engine::GraphicsUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawCallManager::ExecuteDrawCalls();
	RenderSystem::SwapBuffer();
}

void Engine::GameUpdate()
{
	game->Update();
}

GLdouble Engine::deltaTime = 0.0;

ParallelIterator<vec4> Engine::vecIterator;