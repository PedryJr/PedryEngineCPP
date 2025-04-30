#pragma once
#ifndef MyMath
#define MyMath

class PedryMath
{
public:

	static FORCE_INLINE vec2 Lerp(const vec2& A, const vec2& B, float t) {
		return A * t + B * (1.f - t);
	}

	static FORCE_INLINE vec3 Lerp(const vec3& A, const vec3& B, float t) {
		return A * t + B * (1.f - t);
	}

	static FORCE_INLINE vec4 Lerp(const vec4& A, const vec4& B, float t) {
		return A * t + B * (1.f - t);
	}

};
#endif