#pragma once
#include "PedryEngine.h"
Renderer::Renderer()
{
	SetupWindowHints();
	InitializeMonitor();
	OpenGLEnableCaps();
	InitializeWindow();
	DisplaySettings();
	Finalize();
}

void Renderer::DrawShape(VisualShape& toDraw, Vector<vec4>& models)
{

	Shader::EnsureUseProgram(toDraw.GetShader()->GetProgramID());

	glBindVertexArray(toDraw.GetShader()->GetVertexArrayID());

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, toDraw.GetShader()->GetModelBuffer());

	glBufferData(GL_SHADER_STORAGE_BUFFER, INSTANCING_TEST * sizeof(vec4), models.data(), GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, toDraw.GetShader()->GetModelBuffer());

	glDrawElementsInstanced(GL_TRIANGLES, toDraw.GetForm(), GL_UNSIGNED_INT, (void*)0, INSTANCING_TEST);

}

void Renderer::DrawBatch(DrawCallBatch* batch)
{

	Shader::EnsureUseProgram(batch->shader->GetProgramID());
	glBindVertexArray(batch->shader->GetVertexArrayID());

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, batch->shader->GetModelBuffer());
	glBufferData(GL_SHADER_STORAGE_BUFFER, batch->modelMatrices.size() * sizeof(mat4), batch->modelMatrices.data(), GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, batch->shader->GetModelBuffer());
	
	glDrawElementsInstanced(GL_TRIANGLES, batch->mesh->indices.size(), GL_UNSIGNED_INT, (void*)0, batch->modelMatrices.size());

}

void Renderer::SwapBuffer()
{
	glfwSwapBuffers(window);
}

bool Renderer::ShouldTerminate()
{

	return (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

}

void Renderer::SetupWindowHints()
{
	glfwWindowHint(GLFW_SAMPLES, 8);
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

void Renderer::InitializeMonitor()
{
	monitor = glfwGetPrimaryMonitor();
	glfwGetMonitorWorkarea(monitor, NULL, NULL, &width, &height);
}

void Renderer::OpenGLEnableCaps()
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

void Renderer::InitializeWindow()
{
	window = glfwCreateWindow(width, height, "Pedry Engine", NULL, NULL);
	glfwMakeContextCurrent(window);
}

GLFWwindow* Renderer::GetWindow()
{
	return window;
}

void Renderer::DisplaySettings()
{

	glfwSwapInterval(0);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowSize(window, width, height);

}

void Renderer::Finalize()
{
	glClearColor(0.0, 0.0, 0.0, 0.4);
	glewInit();
}

Renderer::~Renderer()
{
	glfwTerminate();
}

void Renderer::Initialize()
{
	SetupWindowHints();
	InitializeMonitor();
	OpenGLEnableCaps();
	InitializeWindow();
	DisplaySettings();
	Finalize();
}

GLFWwindow* Renderer::window = nullptr;
GLFWmonitor* Renderer::monitor = nullptr;
int Renderer::width, Renderer::height = 0;