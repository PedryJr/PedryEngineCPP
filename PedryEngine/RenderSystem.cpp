#pragma once

#include "PedryEngine.h"

void RenderSystem::Initialize()
{
	InitializeMonitor();
	SetupWindowHints();
	InitializeWindow();
	DisplaySettings();
	Finalize();

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
}

void RenderSystem::DrawBatchShadow(DrawCallBatch* batch)
{
	//std::cout << "OPEN GL ERROR CHECK = " << glGetError() << std::endl;
	Shader::EnsureUseProgram(batch->shader->GetShadowID(), batch->shader->GetVertexArrayID());

	glViewport(0, 0, batch->shader->SHADOW_WIDTH, batch->shader->SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, batch->shader->depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);


	batch->shader->UploadShadowLight();
	Shader::EnsureUseProgram(batch->shader->GetShadowID(), batch->shader->GetVertexArrayID());

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, batch->shader->GetModelBuffer());
	glBufferData(GL_SHADER_STORAGE_BUFFER, batch->modelMatrices.size() * sizeof(mat4), batch->modelMatrices.data(), GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, batch->shader->GetModelBuffer());
	
	glDrawElementsInstanced(GL_TRIANGLES, batch->mesh->indices.size(), GL_UNSIGNED_INT, (void*)0, batch->modelMatrices.size());

}

void RenderSystem::DrawBatchNormal(DrawCallBatch* batch)
{

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Shader::EnsureUseProgram(batch->shader->GetProgramID(), batch->shader->GetVertexArrayID());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, batch->shader->depthCubemap);

	GLuint64 handle = glGetTextureHandleARB(batch->shader->depthCubemap);
	glMakeTextureHandleResidentARB(handle);
	batch->shader->SetGLuint64(handle, "shadowMap");

	batch->shader->SetVec3(Engine::lightPos, "lightPos");
	batch->shader->SetFloat(Engine::lightFarPlane, "farPlane");
	Shader::EnsureUseProgram(batch->shader->GetProgramID(), batch->shader->GetVertexArrayID());

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
	

	glfwWindowHint(GLFW_SAMPLES, 4);
	
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
	glDepthRange(0.0f, 1.0f);
	glCullFace(GL_BACK);
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