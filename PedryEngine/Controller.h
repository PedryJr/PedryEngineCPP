#pragma once
class Controller
{

public:


	void ScanInput(GLFWwindow* window, Vector<vec4>& positions);

	static vec2 movementDirection;
	static vec2 mouseDelta;

private:
};