#pragma once

class PedryMath
{
public:

	static vec2 Lerp(const vec2& A, const vec2& B, GLfloat t);

	static vec3 Lerp(const vec3& A, const vec3& B, GLfloat t);

	static vec4 Lerp(const vec4& A, const vec4& B, GLfloat t);

};