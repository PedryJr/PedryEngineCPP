#pragma once
class GameObjectHandle
{


	GLuint objIndex;

public:
	GameObjectHandle();
	GameObjectHandle(GLuint objIndex);
	GameObject& GetObject();
	Transform& GetTransform();

};

