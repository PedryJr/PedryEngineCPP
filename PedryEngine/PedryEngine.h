#pragma once
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#if defined(_MSC_VER)
#define FORCE_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
#define FORCE_INLINE inline __attribute__((always_inline))
#else
#define FORCE_INLINE inline
#endif

#include <immintrin.h>

#include <glm/simd/common.h>
#include <glm/simd/exponential.h>
#include <glm/simd/geometric.h>
#include <glm/simd/integer.h>
#include <glm/simd/matrix.h>
#include <glm/simd/neon.h>
#include <glm/simd/packing.h>
#include <glm/simd/platform.h>
#include <glm/simd/trigonometric.h>
#include <glm/simd/vector_relational.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <thread>
#include <string>
#include <vector>
#include <functional>
#include <cmath>

#include <chrono>
#include <iostream>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "ufbx.h"

#define DEFAULT_SHAPE_COUNT (GLint)9
#define DEFAULT_FORM_COUNT (GLint)3

#define Clock std::chrono::system_clock::time_point
#define TimeNow std::chrono::system_clock::now()
#define Duration(old, fresh) std::chrono::duration<GLdouble>(fresh - old).count()

#define Array std::array
#define Vector std::vector
#define String std::string
#define ToString std::to_string

#define Size size_t
#define FileInfo Stat

#define INSTANCING_TEST (GLint) 1
#define DeltaTime Engine::InnerDeltaTime()
typedef struct stat Stat;

#define vec2 glm::vec2
#define vec3 glm::vec3
#define vec4 glm::vec4
#define quat glm::quat
#define mat4 glm::mat4

struct Triangle {
    vec3 a, b, c;
};

#define vec_UP vec3(0.0F, 1.0F, 0.0F)
#define vec_Right vec3(1.0F, 0.0F, 0.0F)
#define vec_Forward vec3(0.0F, 0.0F, 1.0F)

#define Log(output) std::cout << output << std::endl

#include "TypeDeclare.h"

#include "Shaders.h"
#include "Component.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Shader.h"
#include "DrawCallBatch.h"
#include "DrawCallManager.h"
#include "Transform.h"
#include "AssetManager.h"
#include "GameObject.h"
#include "PedryMath.h"
#include "ParallelIterator.h"
#include "RenderSystem.h"
#include "GlobalCamera.h"
#include "main.h"
#include "Engine.h"
#include "PengTranspiler.h"
#include "ShaderManager.h"
#include "Game.h"
#include "MyRotator.h"
