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

void Game::Start()
{



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
	Mesh* mesh = AssetManager::LoadMesh("Gonagon");

	Shader* shader = new Shader(Vert3D, Frag3D);
	shader->UploadMesh(mesh);
	// Move mesh upload to DrawCallManager?!?
	// Since it only happens when an entirely new draw call is create!
	DrawCallManager::AddDrawCall(mesh, shader, gameObject1->transform);

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
	gameFloor->transform->SetPosition(vec3(0.f, -30.f, 0.f));
	gameFloor->transform->SetScale(vec3(30.f, 5.f, 30.f));

	DrawCallManager::AddDrawCall(mesh, shader, gameObject2->transform);
	DrawCallManager::AddDrawCall(mesh, shader, gameObject3->transform);
	DrawCallManager::AddDrawCall(mesh, shader, gameObject4->transform);
	DrawCallManager::AddDrawCall(mesh, shader, gameObject5->transform);
	DrawCallManager::AddDrawCall(mesh, shader, gameObject6->transform);
	DrawCallManager::AddDrawCall(mesh, shader, gameObject7->transform);
	DrawCallManager::AddDrawCall(mesh, shader, gameObject8->transform);
	DrawCallManager::AddDrawCall(mesh, shader, gameFloor->transform);

	GlobalCamera::aimPositionTarget = vec3(0.f, 0.f, -100.f);

}


void Game::Update()
{
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

}
