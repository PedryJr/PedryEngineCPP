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

	glDrawElementsInstanced(GL_TRIANGLES, toDraw.GetForm(), GL_UNSIGNED_INT, (void*)0, INSTANCING_TEST);

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
	glfwWindowHint(GLFW_SAMPLES, 32);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
	glfwWindowHint(GLFW_RESIZABLE, 1);
}

void Renderer::InitializeMonitor()
{
	monitor = glfwGetPrimaryMonitor();
	glfwGetMonitorWorkarea(monitor, NULL, NULL, &width, &height);
}

void Renderer::OpenGLEnableCaps()
{
	//glEnable(GL_ALPHA_TEST);
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	//glEnable(GL_CULL_FACE);
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