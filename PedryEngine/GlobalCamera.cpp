#pragma once

#include "PedryEngine.h"

GLdouble GlobalCamera::lastX = 0;
GLdouble GlobalCamera::lastY = 0;

vec3 GlobalCamera::aimDirection = vec3(0.0F, 0.0F, 0.0F);
vec3 GlobalCamera::aimDirectionTarget = vec3(0.0F, 0.0F, 0.0F);
vec2 GlobalCamera::aimAngle = vec2(0.0F, 0.0F);
vec3 GlobalCamera::aimPosition = vec3(0.0F, 0.0F, 0.0F);
vec3 GlobalCamera::aimPositionTarget = vec3(0.0F, 0.0F, 0.0F);

bool GlobalCamera::firstMouse = true;