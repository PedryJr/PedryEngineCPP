#pragma once

#include "PedryEngine.h"

RenderSystem::RenderSystem()
{
	SetupWindowHints();
	InitializeMonitor();
	OpenGLEnableCaps();
	InitializeWindow();
	DisplaySettings();
	Finalize();
}

void RenderSystem::DrawBatch(DrawCallBatch* batch)
{

	Shader::EnsureUseProgram(batch->shader->GetProgramID());
	glBindVertexArray(batch->shader->GetVertexArrayID());

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, batch->shader->GetModelBuffer());
	glBufferData(GL_SHADER_STORAGE_BUFFER, batch->modelMatrices.size() * sizeof(mat4), batch->modelMatrices.data(), GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, batch->shader->GetModelBuffer());
	
	glDrawElementsInstanced(GL_TRIANGLES, batch->mesh->indices.size(), GL_UNSIGNED_INT, (void*)0, batch->modelMatrices.size());

}

void RenderSystem::SwapBuffer()
{
	glfwSwapBuffers(window);
}

bool RenderSystem::ShouldTerminate()
{

	return (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

}

void RenderSystem::SetupWindowHints()
{
	glfwWindowHint(GLFW_SAMPLES, 0);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
	glfwWindowHint(GLFW_RESIZABLE, 1);
	glfwWindowHint(GLFW_RED_BITS, 24);
	glfwWindowHint(GLFW_GREEN_BITS, 24);
	glfwWindowHint(GLFW_BLUE_BITS, 24);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);

}

void RenderSystem::InitializeMonitor()
{
	monitor = glfwGetPrimaryMonitor();
	glfwGetMonitorWorkarea(monitor, NULL, NULL, &width, &height);
}

void RenderSystem::OpenGLEnableCaps()
{
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LESS);

	glFrontFace(GL_CCW);

	glDepthMask(GL_TRUE);

}

void RenderSystem::InitializeWindow()
{
	window = glfwCreateWindow(width, height, "Pedry Engine", NULL, NULL);
	glfwMakeContextCurrent(window);
}

GLFWwindow* RenderSystem::GetWindow()
{
	return window;
}

void RenderSystem::DisplaySettings()
{

	glfwSwapInterval(0);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowSize(window, width, height);

}

void RenderSystem::Finalize()
{
	glClearColor(0.0, 0.0, 0.0, 0.4);
	glewInit();
}

RenderSystem::~RenderSystem()
{
	glfwTerminate();
}

void RenderSystem::Initialize()
{
	SetupWindowHints();
	InitializeMonitor();
	InitializeWindow();
	DisplaySettings();
	Finalize();
}

GLFWwindow* RenderSystem::window = nullptr;
GLFWmonitor* RenderSystem::monitor = nullptr;
int RenderSystem::width, RenderSystem::height = 0;