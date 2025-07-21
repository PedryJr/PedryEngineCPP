#include "PedryEngine.h"

ComponentHandle::ComponentHandle() : objIndex(0), compIndex(0)
{
	this->objIndex = 0;
	this->compIndex = 0;
}

ComponentHandle::ComponentHandle(GLuint objIndex, GLuint compIndex)
{
	this->objIndex = objIndex;
	this->compIndex = compIndex;
}

ComponentHandle::~ComponentHandle()
{
}