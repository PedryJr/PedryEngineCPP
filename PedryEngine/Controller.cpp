#pragma once
#include "PedryEngine.h"

void Controller::ScanInput(GLFWwindow* window, Vector<vec4>& positions)
{

	GLboolean didMove = false;

	GLfloat deltaTime = DeltaTime * 10.0;
	GLfloat forwardMod = 0.0f;
	GLfloat rightMod = 0.0f;
	GLfloat sizeMod = 0.0f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) forwardMod += 1.0f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) forwardMod -= 1.0f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) rightMod -= 1.0f;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) rightMod += 1.0f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) sizeMod += 1.0f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) sizeMod -= 1.0f;

	movementDirection = PedryMath::Lerp(movementDirection, vec2(rightMod, forwardMod), deltaTime);


	if (positions.size() <= 0) return;

	
	GLint size = positions.size();
	vec4* location;

	for (GLint i = 1; i < size; i++)
	{
		location = &positions[i];
		positions[i] = PedryMath::Lerp(positions[i], *--location, deltaTime);
	}
	positions[0] = positions[0] + (vec4(movementDirection.x, movementDirection.y, 0, sizeMod) * deltaTime);

}

vec2 Controller::movementDirection = { 0.0f, 0.0f };
vec2 Controller::mouseDelta = { 0.0f, 0.0f };
