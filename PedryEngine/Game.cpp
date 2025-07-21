#pragma once
#include "PedryEngine.h"

GameObjectHandle floor1;

GameObjectHandle spinnerObject;
GameObjectHandle niar;

GameObjectHandle rotator;

float posMag = 100;
float rot = 10;
float floorPos = -250;
float spinner4height = 61;

void MasssiveCubeCountTesst();

void DetailedAvatarTest();

void GimmeFloor();

void AndSomePillars();

void AndSomeSpinnersAroundAPoint(vec3 point);

void FunnyStareAtPlayer();

void Game::Start()
{

	GlobalCamera::aimPositionTarget = vec3(0.f, 50.f, -200.f);

	GimmeFloor();

	DetailedAvatarTest();

	MasssiveCubeCountTesst();

	AndSomePillars();

	FunnyStareAtPlayer();

	for (GLint number = 0; number < 4; number++)
	{
		switch (number)
		{
		case 0: AndSomeSpinnersAroundAPoint(vec3(posMag, spinner4height, posMag));
		case 1: AndSomeSpinnersAroundAPoint(vec3(-posMag, spinner4height, posMag));
		case 2: AndSomeSpinnersAroundAPoint(vec3(-posMag, spinner4height, -posMag));
		case 3: AndSomeSpinnersAroundAPoint(vec3(posMag, spinner4height, -posMag));
		default:
			break;
		}
	}

}

float lightMovement;

void Game::Update()
{

	lightMovement += DeltaTime * 360.f * 0.03f;
	float xRot, zRot;
	xRot = glm::sin(glm::radians(lightMovement)) * 70.f;
	zRot = glm::cos(glm::radians(lightMovement)) * 70.f;
	Engine::lightPos = vec3(xRot, 150.f, zRot);


}

void FunnyStareAtPlayer()
{

	GameObjectHandle handle = GameObject::CreateGameObject();
	handle.GetObject().AddComponent<StareAtPlayerComp>();
	handle.GetObject().AddComponent<Renderer>();
	handle.GetObject().GetComponent<Renderer>().ChangeMesh(AssetManager::LoadMesh("CoolTriThing"));
	handle.GetObject().GetComponent<Transform>().SetPosition(vec3(0.f, 70.f, 0.f));
}

void AndSomeSpinnersAroundAPoint(vec3 point)
{

	GLuint textureSelector = 0;

	GLfloat spinners = 20.f;
	GLfloat rows = 8.f;
	GLfloat rings = 4.f;

	for (GLfloat ring = 1; ring <= rings; ring++)
	{

		for (GLfloat row = 0.f; row < rows; row++)
		{

			for (GLfloat spinner = 0.f; spinner < spinners; spinner++)
			{

				textureSelector = (textureSelector + 1) % 4;

				String tex;

				switch (textureSelector)
				{
				case 0: tex = "Marble"; break;
				case 1: tex = "MegaRed"; break;
				case 2: tex = "MegaWhite"; break;
				case 3: tex = "MegaTex"; break;
				default:
					break;
				}

				Mesh* gonagon = AssetManager::LoadMesh("Gonagon");

				spinnerObject = GameObject::CreateGameObject();
				spinnerObject.GetObject().AddComponent<MyRotator>();
				spinnerObject.GetObject().AddComponent<Renderer>();
				spinnerObject.GetObject().GetComponent<Transform>().SetScale(1.f);
				spinnerObject.GetObject().GetComponent<MyRotator>().offset = spinner / spinners * 8.f;
				spinnerObject.GetObject().GetComponent<MyRotator>().radius = spinners / 20.f + (ring / 3.f);
				spinnerObject.GetObject().GetComponent<MyRotator>().heightOffset = row * 4.f;
				spinnerObject.GetObject().GetComponent<Renderer>().ChangeMesh(gonagon);
				spinnerObject.GetObject().GetComponent<Renderer>().ChangeTexture(AssetManager::LoadTexture(tex));
				spinnerObject.GetObject().GetComponent<MyRotator>().circulationPoint = point;

			}

		}

	}

}

void AndSomePillars()
{

	spinnerObject = GameObject::CreateGameObject();
	spinnerObject.GetObject().GetComponent<Transform>().SetScale(1.f);
	spinnerObject.GetObject().AddComponent<Renderer>();
	spinnerObject.GetObject().GetComponent<Renderer>().ChangeMesh((AssetManager::LoadMesh("Pillar")));
	spinnerObject.GetObject().GetComponent<Renderer>().ChangeTexture(AssetManager::LoadTexture("Marble"));
	spinnerObject.GetObject().GetComponent<Transform>().SetPosition(posMag, floorPos, posMag);
	spinnerObject.GetObject().GetComponent<Transform>().SetScale(rot);

	spinnerObject = GameObject::CreateGameObject();
	spinnerObject.GetObject().GetComponent<Transform>().SetScale(1.f);
	spinnerObject.GetObject().AddComponent<Renderer>();
	spinnerObject.GetObject().GetComponent<Renderer>().ChangeMesh((AssetManager::LoadMesh("Pillar")));
	spinnerObject.GetObject().GetComponent<Renderer>().ChangeTexture(AssetManager::LoadTexture("Marble"));
	spinnerObject.GetObject().GetComponent<Transform>().SetPosition(-posMag, floorPos, posMag);
	spinnerObject.GetObject().GetComponent<Transform>().SetScale(rot);

	spinnerObject = GameObject::CreateGameObject();
	spinnerObject.GetObject().GetComponent<Transform>().SetScale(1.f);
	spinnerObject.GetObject().AddComponent<Renderer>();
	spinnerObject.GetObject().GetComponent<Renderer>().ChangeMesh((AssetManager::LoadMesh("Pillar")));
	spinnerObject.GetObject().GetComponent<Renderer>().ChangeTexture(AssetManager::LoadTexture("Marble"));
	spinnerObject.GetObject().GetComponent<Transform>().SetPosition(posMag, floorPos, -posMag);
	spinnerObject.GetObject().GetComponent<Transform>().SetScale(rot);

	spinnerObject = GameObject::CreateGameObject();
	spinnerObject.GetObject().GetComponent<Transform>().SetScale(1.f);
	spinnerObject.GetObject().AddComponent<Renderer>();
	spinnerObject.GetObject().GetComponent<Renderer>().ChangeMesh((AssetManager::LoadMesh("Pillar")));
	spinnerObject.GetObject().GetComponent<Renderer>().ChangeTexture(AssetManager::LoadTexture("Marble"));
	spinnerObject.GetObject().GetComponent<Transform>().SetPosition(-posMag, floorPos, -posMag);
	spinnerObject.GetObject().GetComponent<Transform>().SetScale(rot);

}

void GimmeFloor()
{
	floor1 = GameObject::CreateGameObject();
	floor1.GetObject().GetComponent<Transform>().SetScale(vec3(300.f, 2.f, 300.f));
	floor1.GetObject().GetComponent<Transform>().SetPosition(vec3(0.f, -50.f, 0.f));
	floor1.GetObject().AddComponent<Renderer>();
	floor1.GetObject().GetComponent<Renderer>().ChangeMesh(AssetManager::LoadMesh("Gonagon"));
	floor1.GetObject().GetComponent<Renderer>().ChangeTexture(AssetManager::LoadTexture("default"));
}

void DetailedAvatarTest()
{

	niar = GameObject::CreateGameObject();
	niar.GetObject().AddComponent<Renderer>();
	niar.GetObject().GetComponent<Renderer>().ChangeMesh(AssetManager::LoadMesh("NiarSINGLE"));
	niar.GetObject().GetComponent<Renderer>().ChangeTexture(AssetManager::LoadTexture("MegaRed"));
	niar.GetObject().GetComponent<Transform>().SetScale(100.f);
	niar.GetObject().GetComponent<Transform>().SetRotation(-90.f, 180.f, 0.f);
	niar.GetObject().GetComponent<Transform>().SetPosition(0.f, -50.f, 0.f);

}

void MasssiveCubeCountTesst()
{

	GLuint textureSelector = 0;

	GLfloat spinners = 250.f;
	GLfloat rows = 4.f;
	GLfloat rings = 4.f;

	for (GLfloat ring = 1; ring <= rings; ring++)
	{

		for (GLfloat row = 0.f; row < rows; row++)
		{

			for (GLfloat spinner = 0.f; spinner < spinners; spinner++)
			{

				textureSelector = (textureSelector + 1) % 4;

				String tex;

				switch (textureSelector)
				{
				case 0: tex = "Marble"; break;
				case 1: tex = "MegaRed"; break;
				case 2: tex = "MegaWhite"; break;
				case 3: tex = "MegaTex"; break;
				default:
					break;
				}

				Mesh* gonagon = AssetManager::LoadMesh("Gonagon");

				spinnerObject = GameObject::CreateGameObject();
				spinnerObject.GetObject().AddComponent<MyRotator>();
				spinnerObject.GetObject().AddComponent<Renderer>();
				spinnerObject.GetObject().GetComponent<Transform>().SetScale(1.f);
				spinnerObject.GetObject().GetComponent<MyRotator>().offset = spinner / spinners * 8.f;
				spinnerObject.GetObject().GetComponent<MyRotator>().radius = spinners / 20.f + (ring / 3.f);
				spinnerObject.GetObject().GetComponent<MyRotator>().heightOffset = row * 4.f;
				spinnerObject.GetObject().GetComponent<Renderer>().ChangeMesh(gonagon);
				spinnerObject.GetObject().GetComponent<Renderer>().ChangeTexture(AssetManager::LoadTexture(tex));

			}

		}

	}


}
