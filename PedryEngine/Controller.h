#pragma once
#ifndef Controller_H
#define Controller_H
class Controller
{

public:
	void ScanInput(GLFWwindow* window, Vector<vec4>& positions);

	vec2 direction;

private:

};
#endif