#pragma once

#include "PedryEngine.h"

GLdouble GlobalCamera::lastX = 0;
GLdouble GlobalCamera::lastY = 0;

vec3 GlobalCamera::aimDirection = vec3(0.0F, 0.0F, 0.0F);
vec3 GlobalCamera::aimDirectionTarget = vec3(0.0F, 0.0F, 0.0F);
vec2 GlobalCamera::aimAngle = vec2(0.0F, 0.0F);
vec3 GlobalCamera::aimPosition = vec3(0.0F, 100.0F, 0.0F);
vec3 GlobalCamera::aimPositionTarget = vec3(0.0F, 0.0F, 0.0F);

mat4 GlobalCamera::viewMatrix = mat4(0.0F);
mat4 GlobalCamera::projectionMatrix = mat4(0.0F);

bool GlobalCamera::firstMouse = true;

GLfloat GlobalCamera::nearPlane = 0.1f;
GLfloat GlobalCamera::farPlane = 1000.0f;

vec2 GlobalCamera::GetMouseDelta()
{

	GLdouble sensitivity = 10.;

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

	deltaX *= sensitivity / 100.;
	deltaY *= sensitivity / 100.;

	return vec2(-deltaX, deltaY);

}

vec3 GlobalCamera::GetInputDirection()
{

	vec3 inputDirection = vec3(0.0F, 0.0F, 0.0F);

	if (glfwGetKey(RenderSystem::GetWindow(), GLFW_KEY_W) == GLFW_PRESS) inputDirection.z += 1.0f;
	if (glfwGetKey(RenderSystem::GetWindow(), GLFW_KEY_S) == GLFW_PRESS) inputDirection.z -= 1.0f;

	if (glfwGetKey(RenderSystem::GetWindow(), GLFW_KEY_A) == GLFW_PRESS) inputDirection.x += 1.0f;
	if (glfwGetKey(RenderSystem::GetWindow(), GLFW_KEY_D) == GLFW_PRESS) inputDirection.x -= 1.0f;

	if (glfwGetKey(RenderSystem::GetWindow(), GLFW_KEY_SPACE) == GLFW_PRESS) inputDirection.y += 1.0f;
	if (glfwGetKey(RenderSystem::GetWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) inputDirection.y -= 1.0f;
	if (glfwGetKey(RenderSystem::GetWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) inputDirection = inputDirection * 2.f;

	return inputDirection;
}

void GlobalCamera::UpdateInput(GLfloat deltaTime)
{

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
	translatedMovement += vec_UP * inputDirection.y;

	aimPositionTarget += translatedMovement * deltaTime * 10.0F;

	aimDirection = PedryMath::Lerp(aimDirection, aimDirectionTarget, deltaTime * 60.0F);
	aimPosition = PedryMath::Lerp(aimPosition, aimPositionTarget, deltaTime * 60.0F);

	vec3 pos, posToDir;
	pos = aimPosition;
	posToDir = pos + aimDirection;

	viewMatrix = glm::lookAt(pos, posToDir, vec_UP);
	SetProjection(47.0f, 16.0f / 9.0f, nearPlane, farPlane);
}

void GlobalCamera::SetProjection(GLfloat vFov, GLfloat aspect, GLfloat near, GLfloat far)
{

	float lerp = glm::smoothstep<float>(0.f, 1.f, float(TimeAlive) / 2.f);
	lerp = glm::smoothstep<float>(0.f, 1.f, lerp);
	lerp = glm::smoothstep<float>(0.f, 1.f, lerp);
	lerp = glm::smoothstep<float>(0.f, 1.f, lerp);
	lerp = glm::smoothstep<float>(0.f, 1.f, lerp);
	lerp = glm::smoothstep<float>(0.f, 1.f, lerp);

	float fov = vFov * lerp;
	projectionMatrix = glm::perspective(glm::radians(fov), aspect, near, far);

}