#pragma once
#include "PedryEngine.h"

#define outerLocation DrawCallManager::drawCalls

#define model outerLocation[batchIndex].gameSwizzle->modelMatrices[modelIndex]


Transform::Transform()
{
	
}

//Transform::Transform(Transform&& other)
//{
//	useContigousArray = other.useContigousArray;
//	position = other.position;
//	rotation = other.rotation;
//	scale = other.scale;
//	shouldUpdateTransform = other.shouldUpdateTransform;
//	batchIndex = other.batchIndex;
//	modelIndex = other.modelIndex;
//}

Transform& Transform::operator=(const Transform& other) = default;

void Transform::Initialize()
{
	this->SetScale(1.f, 1.f, 1.f);
	this->SetPosition(0.f, 0.f, 0.f);
	this->SetRotation(0.f, 0.f, 0.f);
}

void __vectorcall Transform::Simulate()
{
	if (!shouldUpdateTransform) return;
	modelMatrix =
		glm::translate(identityMatrix, position) *
		glm::mat4_cast(rotation) *
		glm::scale(identityMatrix, scale);

	if (useContigousArray) model = modelMatrix;
}

void Transform::Update()
{
}

FORCE_INLINE void Transform::SetPosition(const vec3 position)
{ 
	shouldUpdateTransform = true;
	this->position = { position };
}

void Transform::SetPosition(const GLfloat x, const GLfloat y, const GLfloat z)
{
	SetPosition(vec3(x, y, z));
}

void Transform::SetPosition(const GLfloat scalar)
{
	SetPosition(vec3(scalar, scalar, scalar));
}

FORCE_INLINE void Transform::AddPosition(const vec3 position)
{
	shouldUpdateTransform = true;
	this->position = { this->position + position };
}

void Transform::AddPosition(const GLfloat x, const GLfloat y, const GLfloat z)
{
	AddPosition(vec3(x, y, z));
}

void Transform::AddPosition(const GLfloat scalar)
{
	AddPosition(vec3(scalar, scalar, scalar));
}

FORCE_INLINE void Transform::SetScale(const vec3 scale)
{
	shouldUpdateTransform = true;
	this->scale = { scale };
}

void Transform::SetScale(const GLfloat x, const GLfloat y, const GLfloat z)
{
	SetScale(vec3(x, y, z));
}

void Transform::SetScale(const GLfloat scale)
{
	SetScale(vec3(scale, scale, scale));
}

FORCE_INLINE void Transform::AddScale(const vec3 scale)
{
	shouldUpdateTransform = true;
	this->scale = { this->scale + scale };
}

void Transform::AddScale(const GLfloat x, const GLfloat y, const GLfloat z)
{
	AddScale(vec3(x, y, z));
}

void Transform::AddScale(const GLfloat scale)
{
	AddScale(vec3(scale, scale, scale));
}

FORCE_INLINE void Transform::SetRotation(const quat rotation)
{
	shouldUpdateTransform = true;
	this->rotation = { rotation };
}

void Transform::SetRotation(const GLfloat x, const GLfloat y, const GLfloat z)
{
	SetRotation(glm::qua(glm::radians(vec3(x, y, z))));
}

void Transform::SetRotation(const vec3 euler)
{
	SetRotation(glm::qua(glm::radians(euler)));
}

void Transform::SetRotation(const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat w)
{
	SetRotation(glm::qua(x, y, z, w));
}

void  Transform::SetRotation(const GLfloat angle, const vec3 axis)
{
	SetRotation(glm::qua(glm::radians(angle), axis));
}

void Transform::SetModelLocation(GLint modelIndex, GLint batchIndex)
{
	this->modelIndex = modelIndex;
	this->batchIndex = batchIndex;
}

Vector<DrawCallBatch>& Transform::GetOuterLocation()
{
	return outerLocation;
}

void Transform::Demolish()
{
}
GENERATED_ACTION(Transform)