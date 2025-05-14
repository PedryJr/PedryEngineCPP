#pragma once
#include "PedryEngine.h"

GLfloat lmaoTimer;
Mesh* mesh;
Shader* shader;
GLfloat insaneShit;

GameObject* gameObject;
GameObject* gameObject2;

struct TriangleTest {
    Triangle t1, t2;
    bool expected;
    String description;
};

bool TriangleTriangleIntersect(const Triangle& t1, const Triangle& t2);

void RunTest(const Triangle& t1, const Triangle& t2, bool expected, const String& description) {
    bool hit = TriangleTriangleIntersect(t1, t2);
    std::cout << description << " | Got = " << (hit ? "TRUE" : "FALSE") << ", Expected = " << (expected ? "TRUE" : "FALSE") << std::endl;
}

void RunTests() {

    RunTest(
        { {0,0,0}, {1,0,0}, {0,1,0} },
        { {0.5f,0.5f,-1}, {0.5f,0.5f,1}, {0.5f,1,0} },
        true,
        "Edge intersection: vertical slice through triangle"
    );

    RunTest(
        { {0,0,0}, {1,0,0}, {0,1,0} },
        { {2,2,-1}, {2,2,1}, {2,3,0} },
        false,
        "Clearly separated triangles"
    );

    RunTest(
        { {0,0,0}, {1,0,0}, {0,1,0} },
        { {0.25f,0.25f,0}, {0.75f,0.25f,0}, {0.25f,0.75f,0} },
        true,
        "Coplanar overlap: triangle fully inside another"
    );

    RunTest(
        { {0,0,0}, {1,0,0}, {0,1,0} },
        { {2,0,0}, {3,0,0}, {2,1,0} },
        false,
        "Coplanar no overlap: separate but same plane"
    );

    RunTest(
        { {0,0,0}, {1,0,0}, {0,1,0} },
        { {0.5f,0.5f,0}, {1,1,0}, {1,0.5f,0} },
        true,
        "Coplanar partial overlap"
    );

    RunTest(
        { {0,0,0}, {1,0,0}, {0,1,0} },
        { {-0.5f,-0.5f,-1}, {-0.5f,-0.5f,1}, {-0.5f,0.5f,0} },
        false,
        "Parallel and non-intersecting triangles"
    );

    RunTest(
        { {0,0,0}, {1,0,0}, {0,1,0} },
        { {0,0,0}, {0,0,1}, {0,1,0} },
        true,
        "Shared vertex with depth intersection"
    );

    RunTest(
        { {0,0,0}, {1,0,0}, {0,1,0} },
        { {0,0,0}, {1,1,1}, {1,0,1} },
        true,
        "Shared vertex, oblique triangle"
    );

    RunTest(
        { {0,0,0}, {1,0,0}, {0,1,0} },
        { {0,0,1}, {1,0,1}, {0,1,1} },
        false,
        "Parallel planes, offset by Z"
    );

    RunTest(
        { {0,0,0}, {1,0,0}, {0,1,0} },
        { {0.5f,0.5f,0}, {1.5f,0.5f,0}, {0.5f,1.5f,0} },
        true,
        "Coplanar: Partial overlap with extended triangle"
    );

}

std::mutex mtx;

bool lastWasColliding = false;
bool WasColliding = false;

void AssertCollision(bool& a, bool& b)
{
    //std::lock_guard<std::mutex> lock(mtx);
    b = a || b;
}

void LogLol(bool res) {
    std::cout << "This log wont run!" << res << std::endl;
}

FORCE_INLINE void MoldTriangle(mat4& matrix, Triangle& triangle)
{
    triangle.a = matrix * vec4(triangle.a, 1.0f);
    triangle.b = matrix * vec4(triangle.b, 1.0f);
    triangle.c = matrix * vec4(triangle.c, 1.0f);
}

void RunSpeedTestMTNEW()
{


    ParallelIterator<Triangle> iterator1;
    GLint size = mesh->triangles.size();
    Triangle* arr1 = mesh->triangles.data();

    mat4 matrixA = gameObject->transform->assignedBatch->modelMatrices[gameObject->transform->modelIndex];
    mat4 matrixB = gameObject2->transform->assignedBatch->modelMatrices[gameObject2->transform->modelIndex];
    std::atomic_bool stopCheckIfCondition = false;

    iterator1.setFunction([arr1, size, matrixA, matrixB, &stopCheckIfCondition](Triangle& triangleToTest, GLint index) {
        if (stopCheckIfCondition) return;

        Triangle cachedTriA = triangleToTest;
        cachedTriA.a = matrixA * vec4(cachedTriA.a, 1.0f);
        cachedTriA.b = matrixA * vec4(cachedTriA.b, 1.0f);
        cachedTriA.c = matrixA * vec4(cachedTriA.c, 1.0f);

        for (GLint j = index + 1; j < size; j++) {
            Triangle cachedTriB = arr1[j];
            cachedTriB.a = matrixB * vec4(cachedTriB.a, 1.0f);
            cachedTriB.b = matrixB * vec4(cachedTriB.b, 1.0f);
            cachedTriB.c = matrixB * vec4(cachedTriB.c, 1.0f);

            bool overlap = TriangleTriangleIntersect(cachedTriA, cachedTriB);
            if (overlap) {
                stopCheckIfCondition = true;
                break;
            }
        }
        });



}

void RunSpeedTestMT()
{
    ParallelIterator<Triangle> iterator1 = ParallelIterator<Triangle>();
    GLint collisions = 0;

    GLint size = mesh->triangles.size();

    Triangle* arr1 = mesh->triangles.data();

    bool stopCheckIfCondition = false;
    
    iterator1.setFunction([arr1, size, &stopCheckIfCondition](Triangle& triangleToTest, GLint index) 
    {
        mat4 matrixCache = gameObject->transform->assignedBatch->modelMatrices[gameObject->transform->modelIndex];
        vec4 pointCache = vec4(1);

        Triangle cachedTriA = triangleToTest;
        pointCache.x = cachedTriA.a.x;
        pointCache.y = cachedTriA.a.y;
        pointCache.z = cachedTriA.a.z;
        pointCache.w = 1.0;
        cachedTriA.a = matrixCache * pointCache;

        pointCache.x = cachedTriA.b.x;
        pointCache.y = cachedTriA.b.y;
        pointCache.z = cachedTriA.b.z;
        pointCache.w = 1.0;
        cachedTriA.b = matrixCache * pointCache;

        pointCache.x = cachedTriA.c.x;
        pointCache.y = cachedTriA.c.y;
        pointCache.z = cachedTriA.c.z;
        pointCache.w = 1.0;
        cachedTriA.c = matrixCache * pointCache;

        Triangle cachedTriB;

        //if (stopCheckIfCondition) return;

        for (GLint j = index + 1; j < size - 1; j++)
        {
            matrixCache = gameObject2->transform->assignedBatch->modelMatrices[gameObject2->transform->modelIndex];
            cachedTriB = arr1[j];

            pointCache.x = cachedTriB.a.x;
            pointCache.y = cachedTriB.a.y;
            pointCache.z = cachedTriB.a.z;
            pointCache.w = 1.0;
            cachedTriB.a = matrixCache * pointCache;

            pointCache.x = cachedTriB.b.x;
            pointCache.y = cachedTriB.b.y;
            pointCache.z = cachedTriB.b.z;
            pointCache.w = 1.0;
            cachedTriB.b = matrixCache * pointCache;

            pointCache.x = cachedTriB.c.x;
            pointCache.y = cachedTriB.c.y;
            pointCache.z = cachedTriB.c.z;
            pointCache.w = 1.0;
            cachedTriB.c = matrixCache * pointCache;

            bool overlap = TriangleTriangleIntersect(cachedTriA, cachedTriB);

            AssertCollision(overlap, stopCheckIfCondition);
            //stopCheckIfCondition = TriangleTriangleIntersect(triangleToTest, arr2[j]);
            //if (stopCheckIfCondition) break;
        }

    });

    iterator1.process(arr1, size);
    iterator1.waitForCompletion();
    //LogLol(stopCheckIfCondition);
}
bool TrianglesIntersect(const Triangle& t1, const Triangle& t2);
void RunSpeedTestST()
{

    GLint collisions = 0;

    GLint size = mesh->triangles.size();

    Triangle* arr1 = mesh->triangles.data();

    bool stopCheckIfCondition = false;

    mat4 matrixA = gameObject->transform->GetModelMatrix();
    mat4 matrixB = gameObject2->transform->GetModelMatrix();

    for (GLint index = 0; index < size; index++)
    {
        //if (stopCheckIfCondition) break;
        Triangle& triangleToTest = mesh->triangles.at(index);
        GLint size = mesh->triangles.size();
        Triangle* arr1 = mesh->triangles.data();

        for (GLint j = index + 1; j < size; j++)
        {

            //if (stopCheckIfCondition) break;

            Triangle cachedTriA = triangleToTest;
            MoldTriangle(matrixA, cachedTriA);

            for (GLint j = index + 1; j < size; j++) {
                Triangle cachedTriB = arr1[j];
                MoldTriangle(matrixB, cachedTriB);

                bool overlap = TrianglesIntersect(cachedTriA, cachedTriB);
                stopCheckIfCondition = overlap || stopCheckIfCondition;
            }
        
        }

    }

    WasColliding = stopCheckIfCondition;

}

void Game::Start()
{

	mesh = AssetManager::LoadMesh("Gonagon");
	shader = new Shader(Vert3D, Frag3D);
	shader->UploadMesh(mesh);

	gameObject = new GameObject();
	gameObject2 = new GameObject();
	gameObject->AddComponent<MyRotator>();
	gameObject2->AddComponent<MyRotator>()->offset = 2.5f;

	DrawCallManager::AddDrawCall(mesh, shader, gameObject->transform);
	DrawCallManager::AddDrawCall(mesh, shader, gameObject2->transform);
	//DrawCallManager::RemoveDrawCall(mesh, shader, transform);

	lmaoTimer = 0.0;
	insaneShit = 180;

    RunTests();

    Clock beforeSpeed;
    Clock afterSpeed;
    GLfloat period;

    beforeSpeed = TimeNow;
    RunSpeedTestST();
    afterSpeed = TimeNow;
    
    period = Duration(beforeSpeed, afterSpeed);
    std::cout << "It took " << period << " seconds to scan the mesh using singlethreading!" << std::endl;


    std::cout << mesh->triangles.size() << std::endl;

    //GLuint textureLocation = AssetManager::LoadTexture("default");
    //std::cout << textureLocation << std::endl;

}

void Game::Update()
{
    Clock beforeSpeed;
    Clock afterSpeed;
    GLfloat period;
    beforeSpeed = TimeNow;


	gameObject->Update();
	gameObject2->Update();
    RunSpeedTestST();
    
    afterSpeed = TimeNow;
    period = Duration(beforeSpeed, afterSpeed);
    if (WasColliding != lastWasColliding) {
        lastWasColliding = WasColliding;
        std::cout << "Collision changed to " << (WasColliding ? "TRUE" : "FALSE") << std::endl;
    }

}

#include <immintrin.h>

bool pointInTriangle2D(GLfloat px, GLfloat py,
    GLfloat ax, GLfloat ay, GLfloat bx, GLfloat by, GLfloat cx, GLfloat cy) {
    GLfloat v0x = cx - ax, v0y = cy - ay;
    GLfloat v1x = bx - ax, v1y = by - ay;
    GLfloat v2x = px - ax, v2y = py - ay;

    GLfloat dot00 = v0x * v0x + v0y * v0y;
    GLfloat dot01 = v0x * v1x + v0y * v1y;
    GLfloat dot02 = v0x * v2x + v0y * v2y;
    GLfloat dot11 = v1x * v1x + v1y * v1y;
    GLfloat dot12 = v1x * v2x + v1y * v2y;

    GLfloat denom = dot00 * dot11 - dot01 * dot01;
    if (denom == 0) return false;

    GLfloat u = (dot11 * dot02 - dot01 * dot12) / denom;
    GLfloat v = (dot00 * dot12 - dot01 * dot02) / denom;

    return u >= 0 && v >= 0 && (u + v <= 1); // allows boundary
}


bool linesIntersect(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2,
    GLfloat x3, GLfloat y3, GLfloat x4, GLfloat y4) {
    auto cross = [](GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
        return x1 * y2 - y1 * x2;
        };

    GLfloat dx1 = x2 - x1, dy1 = y2 - y1;
    GLfloat dx2 = x4 - x3, dy2 = y4 - y3;
    GLfloat dx3 = x3 - x1, dy3 = y3 - y1;

    GLfloat denom = cross(dx1, dy1, dx2, dy2);
    if (denom == 0) return false; // parallel or colinear

    GLfloat s = cross(dx3, dy3, dx2, dy2) / denom;
    GLfloat t = cross(dx3, dy3, dx1, dy1) / denom;

    return (s >= 0 && s <= 1 && t >= 0 && t <= 1); // relaxed bounds
}


bool triangleOverlap2D(GLfloat ax, GLfloat ay, GLfloat bx, GLfloat by, GLfloat cx, GLfloat cy,
    GLfloat dx, GLfloat dy, GLfloat ex, GLfloat ey, GLfloat fx, GLfloat fy) {
    if (pointInTriangle2D(ax, ay, dx, dy, ex, ey, fx, fy)) return true;
    if (pointInTriangle2D(bx, by, dx, dy, ex, ey, fx, fy)) return true;
    if (pointInTriangle2D(cx, cy, dx, dy, ex, ey, fx, fy)) return true;

    if (pointInTriangle2D(dx, dy, ax, ay, bx, by, cx, cy)) return true;
    if (pointInTriangle2D(ex, ey, ax, ay, bx, by, cx, cy)) return true;
    if (pointInTriangle2D(fx, fy, ax, ay, bx, by, cx, cy)) return true;

    GLfloat t1x[3] = { ax, bx, cx };
    GLfloat t1y[3] = { ay, by, cy };
    GLfloat t2x[3] = { dx, ex, fx };
    GLfloat t2y[3] = { dy, ey, fy };

    for (GLint i = 0; i < 3; ++i) {
        GLint i2 = (i + 1) % 3;
        for (GLint j = 0; j < 3; ++j) {
            GLint j2 = (j + 1) % 3;
            if (linesIntersect(
                t1x[i], t1y[i], t1x[i2], t1y[i2],
                t2x[j], t2y[j], t2x[j2], t2y[j2]))
                return true;
        }
    }

    return false;
}



bool areCoplanar(const vec3& n1, const vec3& n2, const vec3& offset) {
    constexpr GLfloat epsilon = 1e-5f;
    return glm::abs(glm::dot(n1, n2) - 1.0f) < epsilon &&
        glm::abs(glm::dot(n1, offset)) < epsilon;
}

bool TriangleTriangleIntersect(const Triangle& t1, const Triangle& t2) {
    vec3 e1 = t1.b - t1.a;
    vec3 e2 = t1.c - t1.a;
    vec3 n1 = glm::cross(e1, e2);


    vec3 e3 = t2.b - t2.a;
    vec3 e4 = t2.c - t2.a;
    vec3 n2 = glm::cross(e3, e4);
    
    GLfloat d1 = glm::dot(n1, t2.a - t1.a);
    GLfloat d2 = glm::dot(n1, t2.b - t1.a);
    GLfloat d3 = glm::dot(n1, t2.c - t1.a);

    if ((d1 > 0 && d2 > 0 && d3 > 0) || (d1 < 0 && d2 < 0 && d3 < 0)) {
        return false; // t2 is on one side of t1's plane
    }

    d1 = glm::dot(n2, t1.a - t2.a);
    d2 = glm::dot(n2, t1.b - t2.a);
    d3 = glm::dot(n2, t1.c - t2.a);

    if ((d1 > 0 && d2 > 0 && d3 > 0) || (d1 < 0 && d2 < 0 && d3 < 0)) {
        return false; // t1 is on one side of t2's plane
    }

    // Coplanar check
    if (areCoplanar(n1, n2, t2.a - t1.a)) {
        // Choose projection plane
        vec3 absN = glm::abs(n1);
        int projectionAxis = 0;
        if (absN.y > absN.x) projectionAxis = 1;
        if (absN.z > absN[projectionAxis]) projectionAxis = 2;

        auto project = [&](const vec3& v, GLfloat& u, GLfloat& v_) {
            switch (projectionAxis) {
            case 0: u = v.y; v_ = v.z; break; // project to YZ
            case 1: u = v.x; v_ = v.z; break; // project to XZ
            case 2: u = v.x; v_ = v.y; break; // project to XY
            }
            };

        GLfloat ax, ay, bx, by, cx, cy;
        GLfloat dx, dy, ex, ey, fx, fy;
        project(t1.a, ax, ay);
        project(t1.b, bx, by);
        project(t1.c, cx, cy);
        project(t2.a, dx, dy);
        project(t2.b, ex, ey);
        project(t2.c, fx, fy);

        return triangleOverlap2D(ax, ay, bx, by, cx, cy,
            dx, dy, ex, ey, fx, fy);

    }

    return true;
}

// Helper function: checks whether projections onto a given axis overlap
bool OverlapOnAxis(const Triangle& t1, const Triangle& t2, const vec3& axis) {
    if (glm::length(axis) < 1e-6f) return true; // Degenerate axis

    float t1Min = glm::dot(axis, t1.a);
    float t1Max = t1Min;
    for (vec3 v : {t1.b, t1.c}) {
        float projection = glm::dot(axis, v);
        t1Min = glm::min(t1Min, projection);
        t1Max = glm::max(t1Max, projection);
    }

    float t2Min = glm::dot(axis, t2.a);
    float t2Max = t2Min;
    for (vec3 v : {t2.b, t2.c}) {
        float projection = glm::dot(axis, v);
        t2Min = glm::min(t2Min, projection);
        t2Max = glm::max(t2Max, projection);
    }

    return !(t1Max < t2Min || t2Max < t1Min);
}

bool TrianglesIntersect(const Triangle& t1, const Triangle& t2) {
    // Triangle edges
    vec3 e1[] = { t1.b - t1.a, t1.c - t1.b, t1.a - t1.c };
    vec3 e2[] = { t2.b - t2.a, t2.c - t2.b, t2.a - t2.c };

    // Triangle normals (potential separating axes)
    vec3 n1 = glm::normalize(glm::cross(e1[0], e1[1]));
    vec3 n2 = glm::normalize(glm::cross(e2[0], e2[1]));

    // Test triangle plane normals
    if (!OverlapOnAxis(t1, t2, n1)) return false;
    if (!OverlapOnAxis(t1, t2, n2)) return false;

    // Test edge cross products
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            vec3 axis = glm::cross(e1[i], e2[j]);
            if (!OverlapOnAxis(t1, t2, axis)) return false;
        }
    }

    // All axis tests passed: triangles intersect
    return true;
}