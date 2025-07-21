#pragma once

#include "PedryEngine.h"

void RenderSystem::Initialize()
{

	InitializeMonitor();
	SetupWindowHints();
	InitializeWindow();
	DisplaySettings();
	Finalize();

	std::cout << "Graphics driver: " << "OpenGL - " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Graphics device: " << glGetString(GL_RENDERER) << std::endl;
}

void RenderSystem::PrepareShadowPass() 
{
	glMakeTextureHandleNonResidentARB(Shader::shadowHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, Shader::depthMapFBO);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glViewport(0, 0, Shader::SHADOW_WIDTH, Shader::SHADOW_HEIGHT);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void RenderSystem::DrawBatchShadow(DrawCallBatch* batch)
{
	Shader& shader = *batch->shader->shadowShader;
	Mesh& mesh = *batch->mesh;
	mat4* instanceModels = batch->gameSwizzle->modelMatrices.data();
	GLuint instanceCount = batch->gameSwizzle->modelMatrices.size();
	GLuint elementCount = mesh.indices.size();

	Shader::EnsureUseProgram(shader.GetProgramID());
	Shader::EnsureUseVAO(mesh.GetVAO());

	shader.UploadShadowLight(true);

	Shader::EnsureUseModelMatrixBuffer(shader.GetModelMatrixBuffer());
	glBufferData(GL_SHADER_STORAGE_BUFFER, instanceCount * sizeof(mat4), instanceModels, GL_DYNAMIC_DRAW);
	
	glDrawElementsInstanced(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, (void*)0, instanceCount);
}

void RenderSystem::PrepareNormalPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);
	glReadBuffer(GL_BACK);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Shader::shadowHandle = glGetTextureHandleARB(Shader::depthCubemap);
	glMakeTextureHandleResidentARB(Shader::shadowHandle);
}

void RenderSystem::DrawBatchNormal(DrawCallBatch* batch)
{

	Shader& shader = *batch->shader;

	Mesh& mesh = *batch->mesh;
	mat4* instanceModelsMatrices = batch->gameSwizzle->modelMatrices.data();
	GLuint64* instanceModelMainTextures = batch->gameSwizzle->modelMainTextures.data();
	GLuint64* instanceModelHeightTextures = batch->gameSwizzle->modelHeightTextures.data();
	GLuint instanceCount = batch->gameSwizzle->modelMatrices.size();
	GLuint elementCount = mesh.indices.size();

	Shader::EnsureUseProgram(shader.GetProgramID());
	Shader::EnsureUseVAO(mesh.GetVAO());
	shader.UploadShadowLight(false);

	Shader::EnsureUseModelMatrixBuffer(shader.GetModelMatrixBuffer());
	glBufferData(GL_SHADER_STORAGE_BUFFER, instanceCount * sizeof(mat4), instanceModelsMatrices, GL_DYNAMIC_DRAW);

	Shader::EnsureUseModelMainTextureBuffer(shader.GetModelMainTextureBuffer());
	glBufferData(GL_SHADER_STORAGE_BUFFER, instanceCount * sizeof(GLuint64), instanceModelMainTextures, GL_DYNAMIC_DRAW);

	Shader::EnsureUseModelHeightTextureBuffer(shader.GetModelHeightTextureBuffer());
	glBufferData(GL_SHADER_STORAGE_BUFFER, instanceCount * sizeof(GLuint64), instanceModelHeightTextures, GL_DYNAMIC_DRAW);

	glDrawElementsInstanced(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, (void*)0, instanceCount);

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
	
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, 1);

}

void RenderSystem::InitializeMonitor()
{
	monitor = glfwGetPrimaryMonitor();
	glfwGetMonitorWorkarea(monitor, NULL, NULL, &width, &height);
	width = 2560;
	height = 1440;
}

void RenderSystem::OpenGLEnableCaps()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glCullFace(GL_BACK);

	glDepthFunc(GL_LESS);
}

void RenderSystem::InitializeWindow()
{
	window = glfwCreateWindow(width, height, "Pedry Engine", monitor, NULL);
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
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glewInit();
	OpenGLEnableCaps();
}

GLFWwindow* RenderSystem::window = nullptr;
GLFWmonitor* RenderSystem::monitor = nullptr;
int RenderSystem::width, RenderSystem::height = 0;