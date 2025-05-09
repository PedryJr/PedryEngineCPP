#pragma once
#include "PedryEngine.h"


void Engine::Run()
{
	glfwInit();
	Renderer::Initialize();

	GameObject* gameObject = new GameObject();
	Transform* transform = gameObject->AddComponent<Transform>();
	Transform* transform2 = new Transform();
	Transform* transform3 = new Transform();
	Transform* transform4 = new Transform();
	Transform* transform5 = new Transform();
	Transform* transform6 = new Transform();
	Transform* transform7 = new Transform();
	Transform* transform8 = new Transform();
	Transform* transform9 = new Transform();
	transform->SetScale(vec3(0.5f, 0.5f, 0.5f));
	transform->SetRotation(vec3(-90.0f, 0.0f, 0.0f));

	transform2->SetScale(vec3(1.0f, 1.0f, 1.0f));
	transform3->SetScale(vec3(1.0f, 1.0f, 1.0f));
	transform4->SetScale(vec3(1.0f, 1.0f, 1.0f));
	transform5->SetScale(vec3(1.0f, 1.0f, 1.0f));
	transform2->SetPosition(vec3(1.0f, 0.0f, 1.0f));
	transform3->SetPosition(vec3(1.0f, 1.0f, 0.0f));
	transform4->SetPosition(vec3(0.0f, 1.0f, 1.0f));
	transform5->SetPosition(vec3(2.0f, 1.0f, 2.0f));

	transform6->SetScale(vec3(1.0f, 1.0f, 1.0f));
	transform7->SetScale(vec3(1.0f, 1.0f, 1.0f));
	transform8->SetScale(vec3(1.0f, 1.0f, 1.0f));
	transform9->SetScale(vec3(1.0f, 1.0f, 1.0f));
	transform6->SetPosition(vec3(2.0f, 0.0f, 2.0f));
	transform7->SetPosition(vec3(2.0f, 2.0f, 0.0f));
	transform8->SetPosition(vec3(0.0f, 2.0f, 2.0f));
	transform9->SetPosition(vec3(1.0f, 2.0f, 1.0f));

	transform2->SetPosition(transform2->position + vec3(2.0f, 2.0f, 2.0f));
	transform->SetPosition(transform->position - vec3(2.0f, 2.0f, 2.0f));

	Mesh* mesh = AssetManager::LoadMesh("NiarSINGLE");
	Shader* shader = new Shader(Vert3D, Frag3D);
	shader->UploadShape(mesh->vertices.data(), mesh->indices.data(), mesh->normals.data(), mesh->uvs.data(), mesh->tangents.data(), mesh->vertices.size(), mesh->indices.size());
	DrawCallManager::AddDrawCall(mesh, shader, transform);
	DrawCallManager::AddDrawCall(mesh, shader, transform2);
	DrawCallManager::AddDrawCall(mesh, shader, transform3);
	DrawCallManager::AddDrawCall(mesh, shader, transform4);
	DrawCallManager::AddDrawCall(mesh, shader, transform5);
	DrawCallManager::AddDrawCall(mesh, shader, transform6);
	DrawCallManager::AddDrawCall(mesh, shader, transform7);
	DrawCallManager::AddDrawCall(mesh, shader, transform8);
	DrawCallManager::AddDrawCall(mesh, shader, transform9);

	Vector<vec4> positions(INSTANCING_TEST);

	Clock timer = TimeNow;

	GLfloat lmaoTimer = 0.0;

	Renderer::OpenGLEnableCaps();

	while (Renderer::ShouldTerminate())
	{

		GLfloat insaneShit = 2000;

		lmaoTimer += deltaTime;
		transform->SetRotation(vec3(glm::sin(lmaoTimer) * insaneShit, 0.0f, 0.0f));
		transform2->SetRotation(vec3(glm::sin(lmaoTimer + 0.2f) * insaneShit, 0.0f, 0.0f));
		transform3->SetRotation(vec3(glm::sin(lmaoTimer + 0.6f) * insaneShit, 0.0f, 0.0f));
		transform4->SetRotation(vec3(glm::sin(lmaoTimer + 1.0f) * insaneShit, 0.0f, 0.0f));
		transform5->SetRotation(vec3(glm::sin(lmaoTimer + 1.4f) * insaneShit, 0.0f, 0.0f));
		transform6->SetRotation(vec3(glm::sin(lmaoTimer + 1.8f) * insaneShit, 0.0f, 0.0f));
		transform7->SetRotation(vec3(glm::sin(lmaoTimer + 2.2f) * insaneShit, 0.0f, 0.0f));
		transform8->SetRotation(vec3(glm::sin(lmaoTimer + 2.6f) * insaneShit, 0.0f, 0.0f));
		transform9->SetRotation(vec3(glm::sin(lmaoTimer + 3.0f) * insaneShit, 0.0f, 0.0f));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GlobalCamera::UpdateInput(deltaTime);

		transform2->Update();
		transform3->Update();
		transform4->Update();
		transform5->Update();
		transform6->Update();
		transform7->Update();
		transform8->Update();
		transform9->Update();
		transform->Update();
		//gameObject->Update();
		DrawCallManager::ExecuteDrawCalls();


		Renderer::SwapBuffer();

		glfwPollEvents();

		Clock newTimer = TimeNow;
		deltaTime = Duration(timer, newTimer);
		timer = newTimer;
	}
}

GLdouble Engine::deltaTime = 0.0;

ParallelIterator<vec4> Engine::vecIterator;