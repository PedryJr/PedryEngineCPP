#pragma once
#include "PedryEngine.h"
void Controller::ScanInput(GLFWwindow* window, Vector<vec4>& positions)
{

	GLboolean didMove = false;

	GLfloat deltaTime = DeltaTime * 10.0;
	GLfloat forwardMod = 0.0f;
	GLfloat rightMod = 0.0f;
	GLfloat sizeMod = 0.0f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		forwardMod += 1.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		forwardMod -= 1.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		rightMod -= 1.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		rightMod += 1.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		sizeMod += 1.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		sizeMod -= 1.0f;
	}

	direction = PedryMath::Lerp(direction, vec2(rightMod, forwardMod), deltaTime);


	if (positions.size() <= 0) return;

	positions[0] = positions[0] + (vec4(direction.x, direction.y, 0, sizeMod) * deltaTime);

	GLint size = positions.size();
	for (GLint i = 1; i < size; i++)
	{
		
		vec4* location = &positions[i];
		positions[i] = PedryMath::Lerp(positions[i], *--location, deltaTime);

	}


}
