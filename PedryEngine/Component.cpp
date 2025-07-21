#pragma once
#include "PedryEngine.h"
GLuint Component::componentIDGenerator = 0;

GameObject& Component::GetGameObject()
{
	return GameObject::objArr[owner];
}
