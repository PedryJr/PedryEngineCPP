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
	//Drawcall data.
	Shader& shader = *batch->shader->shadowShader;
	Mesh& mesh = *batch->mesh;
	mat4* instanceModels = batch->modelMatrices.data();
	GLuint instanceCount = batch->modelMatrices.size();
	GLuint elementCount = mesh.indices.size();

	shader.UploadShadowLight(true);
	Shader::EnsureUseModelMatrixBuffer(shader.GetModelMatrixBuffer());
	Shader::EnsureUseProgram(shader.GetProgramID());
	Shader::EnsureUseVAO(mesh.GetVAO());

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
	mat4* instanceModelsMatrices = batch->modelMatrices.data();
	GLuint64* instanceModelMainTextures = batch->modelMainTextures.data();
	GLuint64* instanceModelHeightTextures = batch->modelHeightTextures.data();
	GLuint instanceCount = batch->modelMatrices.size();
	GLuint elementCount = mesh.indices.size();

	shader.UploadShadowLight(false);
	Shader::EnsureUseProgram(shader.GetProgramID());
	Shader::EnsureUseVAO(mesh.GetVAO());

	Shader::EnsureUseModelMatrixBuffer(shader.GetModelMatrixBuffer());
	glBufferData(GL_SHADER_STORAGE_BUFFER, instanceCount * sizeof(mat4), instanceModelsMatrices, GL_DYNAMIC_DRAW);

	Shader::EnsureUseModelMainTextureBuffer(shader.GetModelMainTextureBuffer());
	glNamedBufferStorage(shader.GetModelMainTextureBuffer(), sizeof(GLuint64) * instanceCount, (const void*)instanceModelMainTextures, GL_DYNAMIC_STORAGE_BIT);

	Shader::EnsureUseModelHeightTextureBuffer(shader.GetModelHeightTextureBuffer());
	glNamedBufferStorage(shader.GetModelMainTextureBuffer(), sizeof(GLuint64) * instanceCount, (const void*)instanceModelHeightTextures, GL_DYNAMIC_STORAGE_BIT);

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
	

	glfwWindowHint(GLFW_SAMPLES, 8);
	
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Set the windowed mode hints
	glfwWindowHint(GLFW_RESIZABLE, 1);
	//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);

	//Coor bits

}

void RenderSystem::InitializeMonitor()
{
	monitor = glfwGetPrimaryMonitor();
	glfwGetMonitorWorkarea(monitor, NULL, NULL, &width, &height);
}

void RenderSystem::OpenGLEnableCaps()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

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
	OpenGLEnableCaps();
	glClearColor(0.0, 0.0, 0.0, 0.4);
	glewInit();
}

GLFWwindow* RenderSystem::window = nullptr;
GLFWmonitor* RenderSystem::monitor = nullptr;
int RenderSystem::width, RenderSystem::height = 0;