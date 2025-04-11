#pragma once
#include "PedryEngine.h"

vec2 PedryMath::Lerp(const vec2& A, const vec2& B, float t) {
	return A * t + B * (1.f - t);
}

vec3 PedryMath::Lerp(const vec3& A, const vec3& B, float t) {
	return A * t + B * (1.f - t);
}

vec4 PedryMath::Lerp(const vec4& A, const vec4& B, float t) {
	return A * t + B * (1.f - t);
}