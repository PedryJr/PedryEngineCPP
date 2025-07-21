#include "PedryEngine.h"

GameObjectHandle::GameObjectHandle()
{
}

GameObjectHandle::GameObjectHandle(GLuint objIndex)
{
	this->objIndex = objIndex;
}

GameObject& GameObjectHandle::GetObject()
{
	return GameObject::objArr[objIndex];
}