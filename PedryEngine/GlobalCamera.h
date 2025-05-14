#pragma once
class GlobalCamera
{
public:
	static GLdouble lastX, lastY;

	static vec3 aimPosition;
	static vec3 aimDirection;

	static vec3 aimPositionTarget;
	static vec3 aimDirectionTarget;

	static vec2 aimAngle;
	static bool firstMouse;

	static vec2 GetMouseDelta() {

		GLdouble deltaX, deltaY;

		GLdouble currentX;
		GLdouble currentY;

		glfwGetCursorPos(RenderSystem::GetWindow(), &currentX, &currentY);

		if (firstMouse) {
			lastX = currentX;
			lastY = currentY;
			firstMouse = false;
		}

		deltaX = currentX - lastX;
		deltaY = currentY - lastY;

		lastX = currentX;
		lastY = currentY;

		return vec2(-deltaX, deltaY) / 10.0f;

	}

	static vec3 GetInputDirection() {

		vec3 inputDirection = vec3(0.0F, 0.0F, 0.0F);

		if (glfwGetKey(RenderSystem::GetWindow(), GLFW_KEY_W) == GLFW_PRESS) inputDirection.z += 1.0f;
		if (glfwGetKey(RenderSystem::GetWindow(), GLFW_KEY_S) == GLFW_PRESS) inputDirection.z -= 1.0f;

		if (glfwGetKey(RenderSystem::GetWindow(), GLFW_KEY_A) == GLFW_PRESS) inputDirection.x += 1.0f;
		if (glfwGetKey(RenderSystem::GetWindow(), GLFW_KEY_D) == GLFW_PRESS) inputDirection.x -= 1.0f;
		return inputDirection;
	}

	static void UpdateInput(GLfloat deltaTime) {

		GLFWwindow* window = RenderSystem::GetWindow();

		vec3 inputDirection = GetInputDirection();
		vec2 mouseDelta = GetMouseDelta();

		aimAngle += mouseDelta / 100.0f;

		vec3 right = vec3(
			sin(aimAngle.x - 3.14f / 2.0f),
			0.0F,
			cos(aimAngle.x - 3.14f / 2.0f)
		);

		aimDirectionTarget = vec3(
			glm::cos(aimAngle.y) * glm::sin(aimAngle.x),
			-glm::sin(aimAngle.y),
			glm::cos(aimAngle.y) * cos(aimAngle.x));

		vec3 translatedMovement(0.0F, 0.0F, 0.0F);
		translatedMovement += aimDirection * inputDirection.z;
		translatedMovement += -right * inputDirection.x;

		aimPositionTarget += translatedMovement * deltaTime * 10.0F;

		aimDirection = PedryMath::Lerp(aimDirection, aimDirectionTarget, deltaTime * 57.0F);
		aimPosition = PedryMath::Lerp(aimPosition, aimPositionTarget, deltaTime * 57.0F);

	}

	static mat4 GetViewMatrix() {

		vec3 pos, posToDir;
		pos = aimPosition;
		posToDir = pos + aimDirection;

		mat4 view = glm::lookAt(pos , posToDir, vec_UP);

		return view;

	}

};

