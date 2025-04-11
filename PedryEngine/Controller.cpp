#pragma once
#include "PedryEngine.h"
void Controller::ScanInput(GLFWwindow* window, Vector<vec4>& positions)
{

	bool didMove = false;

	float deltaTime = DeltaTime * 10.0f;
	float forwardMod = 0.0f;
	float rightMod = 0.0f;
	float sizeMod = 0.0f;

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
	positions[0].z = glm::abs(direction.y) + glm::abs(direction.x) + (15.0f);

	Engine::vecIterator.setFunction([deltaTime](vec4& value) {

		vec4* location = &value;

		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);
		value = PedryMath::Lerp(value, *--location, deltaTime);

		});

	Engine::vecIterator.process(&positions[1], INSTANCING_TEST-1);
	Engine::vecIterator.waitForCompletion();


}
