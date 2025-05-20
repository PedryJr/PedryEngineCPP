#pragma once
#include "PedryEngine.h"

vec2 PedryMath::Lerp(const vec2& A, const vec2& B, GLfloat t)
{
	t = glm::fclamp<GLfloat>(t, 0.F, 1.F);
	return B * t + A * (1.F - t);
}

vec3 PedryMath::Lerp(const vec3& A, const vec3& B, GLfloat t)
{
	t = glm::fclamp<GLfloat>(t, 0.F, 1.F);
	return B * t + A * (1.F - t);
}

vec4 PedryMath::Lerp(const vec4& A, const vec4& B, GLfloat t)
{
	t = glm::fclamp<GLfloat>(t, 0.F, 1.F);
	return B * t + A * (1.F - t);
}
