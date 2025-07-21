#pragma once

class Component
{
public:
	//virtual ~Component() {}
	//virtual void Initialize() {}
	//virtual void Demolish() {}
	//virtual void Simulate() {}
	//virtual void Update() {}

	static GLuint componentIDGenerator;
	GLuint componentID;
	ComponentHandle handle;
	GLuint ecsGhost;
	GameObject& GetGameObject();
	GLuint owner;
private:
};