#pragma once

class ComponentHandle
{

	GLuint compIndex;
	GLuint objIndex;

public:

	ComponentHandle();
	ComponentHandle(GLuint objIndex, GLuint compIndex);

	~ComponentHandle();
	template<typename T>
	T& GetComponent();

};