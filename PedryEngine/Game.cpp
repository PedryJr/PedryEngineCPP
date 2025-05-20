#pragma once
#include "PedryEngine.h"

GameObject* gameObject1;
GameObject* gameObject2;
GameObject* gameObject3;
GameObject* gameObject4;
GameObject* gameObject5;
GameObject* gameObject6;
GameObject* gameObject7;
GameObject* gameObject8;
GameObject* gameFloor;

GameObject* pillar1;
GameObject* pillar2;
GameObject* pillar3;
GameObject* pillar4;
GameObject* pillar5;
GameObject* pillar6;
GameObject* pillar7;
GameObject* pillar8;
GameObject* pillar9;
GameObject* pillar10;
GameObject* pillar11;
GameObject* pillar12;

GameObject* lamp;

void Game::Start()
{

	//Example on how to preload assets.
	AssetManager::LoadMesh("CoolTriThing");
	AssetManager::LoadTexture("megumin");
	AssetManager::LoadMesh("Test");
	AssetManager::LoadShader("default");
	AssetManager::LoadMesh("Gonagon");
	AssetManager::LoadMesh("Test2");
	AssetManager::LoadMesh("DoughNut");
	AssetManager::LoadTexture("default");
	AssetManager::LoadShader("Shadow");
	AssetManager::LoadMesh("Sphere");
	AssetManager::LoadTexture("tohru");
	AssetManager::LoadMesh("NiarSINGLE");
	AssetManager::LoadMesh("Pillar");



	// How i want the front end to look!
	// Self explanatory and scalable with a clean entry point.
	// MyRotator is an example component i created for testing purposes.
	gameObject1 = new GameObject();

	gameObject1->AddComponent<MyRotator>()->offset = 0.0f;

	// TODO:
	// I need to create systems for dynamic loading of meshes, textures, shader compilation and the drawcalls assigned.
	// Reuse of memory here is essential, maybe extract all except drawcalls to the Asset Manager.
	// Render component might be the an addition to the solution?
	// Possible render component should only comunicate the wish/desire to be seen, the engine should respond accordingly!
	// Render component should be aware of some things.
	// Texture handle, Mesh ID, Shader ID. (Variant/Instance, Unique/Draw separation, Unique/Draw separation) 
	// Render component notifies DrawCallManager -> DrawCallManager handles what batch the renderer belongs to.
	// DrawCallManager should be able to swap instances between batches. 
	// (It can already add and remove, but preferably using Render component to identify its Uniqueness)
	// 
	// For now, objects can be added to the graphics pipeline by using the basic managers and objects that currently exist.
	Mesh* triThing = AssetManager::LoadMesh("CoolTriThing");
	Mesh* gonagon = AssetManager::LoadMesh("Gonagon");
	Mesh* sphere = AssetManager::LoadMesh("Sphere");
	Mesh* pillar = AssetManager::LoadMesh("Pillar");

	for (GLfloat& normal : sphere->normals) normal = -normal;
	sphere->UpdateGpuStorage();

	Shader* shader = AssetManager::LoadShader("default");
	Shader* shader2 = AssetManager::LoadShader("default");
	// Move mesh upload to DrawCallManager?!?
	// Since it only happens when an entirely new draw call is create!
	DrawCallManager::AddDrawCall(triThing, shader, gameObject1->transform);

	// Also...
	// OOps.. 
	// memory leak :p 
	// (Non serious joke, please future me dont read too much into this)

	//The rest of this code is mostly just to get a cool scene.
	gameObject2 = new GameObject();
	gameObject3 = new GameObject();
	gameObject4 = new GameObject();
	gameObject5 = new GameObject();
	gameObject6 = new GameObject();
	gameObject7 = new GameObject();
	gameObject8 = new GameObject();

	gameObject2->AddComponent<MyRotator>()->offset = 1.0f;
	gameObject3->AddComponent<MyRotator>()->offset = 2.0f;
	gameObject4->AddComponent<MyRotator>()->offset = 3.0f;
	gameObject5->AddComponent<MyRotator>()->offset = 4.0f;
	gameObject6->AddComponent<MyRotator>()->offset = 5.0f;
	gameObject7->AddComponent<MyRotator>()->offset = 6.0f;
	gameObject8->AddComponent<MyRotator>()->offset = 7.0f;

	gameFloor = new GameObject();
	gameFloor->transform->SetPosition(vec3(0.f, -50.f, 0.f));
	gameFloor->transform->SetScale(vec3(400.f, 5.f, 400.f));

	DrawCallManager::AddDrawCall(triThing, shader, gameObject2->transform);
	DrawCallManager::AddDrawCall(triThing, shader2, gameObject3->transform);
	DrawCallManager::AddDrawCall(triThing, shader, gameObject4->transform);
	DrawCallManager::AddDrawCall(triThing, shader, gameObject5->transform);
	DrawCallManager::AddDrawCall(triThing, shader, gameObject6->transform);
	DrawCallManager::AddDrawCall(triThing, shader, gameObject7->transform);
	DrawCallManager::AddDrawCall(triThing, shader, gameObject8->transform);
	DrawCallManager::AddDrawCall(gonagon, shader2, gameFloor->transform);

	GlobalCamera::aimPositionTarget = vec3(0.f, 0.f, -100.f);

	gameObject1->transform->SetScale(vec3(3.8f));
	gameObject5->transform->SetScale(vec3(3.8f));
	gameObject2->transform->SetScale(vec3(3.8f));
	gameObject3->transform->SetScale(vec3(3.8f));
	gameObject4->transform->SetScale(vec3(3.8f));
	gameObject6->transform->SetScale(vec3(3.8f));
	gameObject7->transform->SetScale(vec3(3.8f));
	gameObject8->transform->SetScale(vec3(3.8f));

	float pillarY = -45.f;

	lamp = new GameObject();
	lamp->transform->SetScale(vec3(1.f));
	DrawCallManager::AddDrawCall(sphere, shader2, lamp->transform);

	pillar1 = new GameObject();
	pillar1->transform->SetPosition(30, pillarY, 30);
	pillar1->transform->SetScale(10);

	pillar2 = new GameObject();
	pillar2->transform->SetPosition(-30, pillarY, 30);
	pillar2->transform->SetScale(10);

	pillar3 = new GameObject();
	pillar3->transform->SetPosition(-30, pillarY, -30);
	pillar3->transform->SetScale(10);

	pillar4 = new GameObject();
	pillar4->transform->SetPosition(30, pillarY, -30);
	pillar4->transform->SetScale(10);

	pillar5 = new GameObject();
	pillar5->transform->SetPosition(60, pillarY, 90);
	pillar5->transform->SetScale(10);

	pillar6 = new GameObject();
	pillar6->transform->SetPosition(-60, pillarY, 90);
	pillar6->transform->SetScale(10);

	pillar7 = new GameObject();
	pillar7->transform->SetPosition(-60, pillarY, -90);
	pillar7->transform->SetScale(10);

	pillar8 = new GameObject();
	pillar8->transform->SetPosition(60, pillarY, -90);
	pillar8->transform->SetScale(10);

	pillar9 = new GameObject();
	pillar9->transform->SetPosition(90, pillarY, 60);
	pillar9->transform->SetScale(10);

	pillar10 = new GameObject();
	pillar10->transform->SetPosition(-90, pillarY, 60);
	pillar10->transform->SetScale(10);

	pillar11 = new GameObject();
	pillar11->transform->SetPosition(-90, pillarY, -60);
	pillar11->transform->SetScale(10);

	pillar12 = new GameObject();
	pillar12->transform->SetPosition(90, pillarY, -60);
	pillar12->transform->SetScale(10);

	DrawCallManager::AddDrawCall(pillar, shader, pillar1->transform);
	DrawCallManager::AddDrawCall(pillar, shader, pillar2->transform);
	DrawCallManager::AddDrawCall(pillar, shader, pillar3->transform);
	DrawCallManager::AddDrawCall(pillar, shader, pillar4->transform);
	DrawCallManager::AddDrawCall(pillar, shader, pillar5->transform);
	DrawCallManager::AddDrawCall(pillar, shader, pillar6->transform);
	DrawCallManager::AddDrawCall(pillar, shader, pillar7->transform);
	DrawCallManager::AddDrawCall(pillar, shader, pillar8->transform);
	DrawCallManager::AddDrawCall(pillar, shader2, pillar9->transform);
	DrawCallManager::AddDrawCall(pillar, shader2, pillar10->transform);
	DrawCallManager::AddDrawCall(pillar, shader2, pillar11->transform);
	DrawCallManager::AddDrawCall(pillar, shader2, pillar12->transform);

	pillar->UpdateGpuStorage();

}

GLfloat testTimer = 0.f;

GLfloat yPos = 0.f;

void Game::Update()
{

	//GetKeyArrowUpDown to set yPos up down using glfw.
	if (glfwGetKey(RenderSystem::GetWindow(), GLFW_KEY_UP) == GLFW_PRESS) yPos += DeltaTime * 15;
	if (glfwGetKey(RenderSystem::GetWindow(), GLFW_KEY_DOWN) == GLFW_PRESS) yPos -= DeltaTime * 15;


	testTimer += DeltaTime / 10;

	Engine::lightPos = vec3(0.f, -30.0f + yPos, 0.f);
	lamp->transform->SetPosition(Engine::lightPos);

	//Example on how i want an observer to iterate on and execute code on all GameObjects. (For my own sanity)

	//Run multithreaded functions, pro tip, dont touch others.
    gameObject1->Simulate();
    gameObject2->Simulate();
    gameObject3->Simulate();
    gameObject4->Simulate();
    gameObject5->Simulate();
    gameObject6->Simulate();
    gameObject7->Simulate();
    gameObject8->Simulate();
    gameFloor->Simulate();
	lamp->Simulate();
	pillar1->Simulate();
	pillar2->Simulate();
	pillar3->Simulate();
	pillar4->Simulate();
	pillar5->Simulate();
	pillar6->Simulate();
	pillar7->Simulate();
	pillar8->Simulate();
	pillar9->Simulate();
	pillar10->Simulate();
	pillar11->Simulate();
	pillar12->Simulate();


	//Run the normal update loop.
	gameObject1->Update();
    gameObject2->Update();
    gameObject3->Update();
    gameObject4->Update();
    gameObject5->Update();
    gameObject6->Update();
    gameObject7->Update();
    gameObject8->Update();
	gameFloor->Update();
	lamp->Update();
	pillar1->Update();
	pillar2->Update();
	pillar3->Update();
	pillar4->Update();
	pillar5->Update();
	pillar6->Update();
	pillar7->Update();
	pillar8->Update();
	pillar9->Update();
	pillar10->Update();
	pillar11->Update();
	pillar12->Update();

}
