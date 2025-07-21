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
#include <fstream>
#include <sstream>
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <deque>
#include <future>
#include <stdexcept>
#include <typeinfo>

#define GLM_FORCE_INTRINSICS
#define GLM_FORCE_SIMD_AVX2 // or SSE2, SSE4_1, etc., depending on target

#include <glm/glm.hpp>                  // Core types: vec2, vec3, vec4, mat4, etc.
#include <glm/gtc/quaternion.hpp>       // Quaternions
#include <glm/gtc/matrix_transform.hpp> // Transform functions
#include <glm/gtc/type_ptr.hpp>         // Conversion to/from raw pointers

#include <gl/glew.h>
#include <GLFW/glfw3.h>

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
#define TimeAlive Engine::InnerTimeAlive()
typedef struct stat Stat;

#define vec2 glm::vec2
#define vec3 glm::vec3
#define vec4 glm::vec4
#define quat glm::quat
#define mat4 glm::mat4

#define ReadFileToString(path, outStr, resultBool)                 \
    {                                                              \
        std::ifstream f(path);                                     \
        if (!f.is_open()) resultBool = false;                      \
        else {                                                     \
            std::ostringstream s;                                  \
            s << f.rdbuf();                                        \
            outStr = s.str();                                      \
            resultBool = true;                                     \
        }                                                          \
    }



struct WorldTriangle {
    mat4 oldMatrix;
    vec3 localA, localB, localC;
    vec3 globalA, globalB, globalC;
    vec3 aabbMin, aabbMax;

    void UpdateForm(const mat4& modelMatrix) 
    {
        if (oldMatrix == modelMatrix) return;
        globalA = modelMatrix * vec4(localA, 1.0f);
        globalB = modelMatrix * vec4(localB, 1.0f);
        globalC = modelMatrix * vec4(localC, 1.0f);

        aabbMin = glm::min(globalA, glm::min(globalB, globalC));
        aabbMax = glm::max(globalA, glm::max(globalB, globalC));
        oldMatrix = modelMatrix;
    }
};

#define vec_UP vec3(0.0F, 1.0F, 0.0F)
#define vec_Right vec3(1.0F, 0.0F, 0.0F)
#define vec_Forward vec3(0.0F, 0.0F, 1.0F)

#define Log(output) std::cout << output << std::endl

#define LogGL()\
{\
    switch (glGetError())\
    {\
    case GL_NO_ERROR:          Log("No Error"); break;\
    case GL_INVALID_ENUM:      Log("Invalid Enum"); break;\
    case GL_INVALID_VALUE:     Log("Invalid Value"); break;\
    case GL_INVALID_OPERATION: Log("Invalid Operation"); break;\
    case GL_INVALID_FRAMEBUFFER_OPERATION: Log("Invalid Framebuffer Operation"); break;\
    case GL_OUT_OF_MEMORY:     Log("Out of Memory"); break;\
    case GL_STACK_UNDERFLOW:   Log("Stack Underflow"); break;\
    case GL_STACK_OVERFLOW:    Log("Stack Overflow"); break;\
    case GL_CONTEXT_LOST:      Log("Context Lost"); break;\
    default:                   Log("Unknown Error");\
    }\
}

#define GENERATED_CLASS(ComponentType) \
template class ParallelIterator<ComponentType>;

#define GENERATED_ACTION(ComponentType)                                     \
Vector<ComponentType> ComponentType::componentArray;                        \
GLuint ComponentType::type = typeid(ComponentType).hash_code(); \
ParallelIterator<ComponentType> ComponentType::compIterator = ParallelIterator<ComponentType>();

#define GENERATED_BODY(ComponentType)                                       \
public:                                                                     \
    static ParallelIterator<ComponentType> compIterator;                    \
    static GLuint type;                                                     \
    static Vector<ComponentType> componentArray;                            \
    static void ProcessAll() {                                              \
        compIterator.process(componentArray.data(), componentArray.size());\
    }                                                                       \
    static void UpdateAll() {                                                   \
                                                                            \
        for (auto& comp : componentArray) comp.Update();                    \
    }                                                                           \
    static void WaitAll() {                                                   \
                                                                            \
        compIterator.waitForCompletion();                                      \
    }                                                                           \
    static void CompInit()                                                      \
    {                                                                           \
                                                                                \
        compIterator.setFunction([](ComponentType& comp, const GLint index) \
            {                                                                       \
                comp.Simulate();                                                    \
            } );                                                                    \
        GLuint ecsGhost = componentArray.size();                                   \
        componentArray.emplace_back();                                          \
        componentArray[ecsGhost].ecsGhost = ecsGhost;                          \
    }

#include "TypeDeclare.h"

#include "ComponentHandle.h"
#include "GameObjectHandle.h"
#include "ParallelIterator.h"
#include "ThreadPool.h"
#include "Texture.h"
#include "Shaders.h"
#include "Component.h"
#include "MyMover.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Shader.h"
#include "DrawCallBatch.h"
#include "DrawCallManager.h"
#include "Transform.h"
#include "AssetManager.h"
#include "GameObject.h"
#include "PedryMath.h"
#include "RenderSystem.h"
#include "GlobalCamera.h"
#include "main.h"
#include "Engine.h"
#include "PengTranspiler.h"
#include "ShaderManager.h"
#include "Game.h"
#include "MyRotator.h"
#include "StareAtPlayerComp.h"

template<typename T>
T& ComponentHandle::GetComponent()
{
    return T::componentArray[compIndex];
}