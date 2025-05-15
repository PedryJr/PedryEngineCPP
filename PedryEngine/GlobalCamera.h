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

	static mat4 projectionMatrix;
	static mat4 viewMatrix;

	static vec2 GetMouseDelta();

	static vec3 GetInputDirection();

	static void UpdateInput(GLfloat deltaTime);

	static void SetProjection(GLfloat vFov, GLfloat aspect, GLfloat near, GLfloat far);

};

