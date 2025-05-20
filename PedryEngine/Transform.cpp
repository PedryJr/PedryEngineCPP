#pragma once

#include "PedryEngine.h"

void Transform::Initialize()
{
	position = vec3(0.0f, 0.0f, 0.0f);
	scale = vec3(1.0f, 1.0f, 1.0f);
	this->rotation = mat4(1.0f);
}

void Transform::Simulate()
{
}

void Transform::Update()
{
	UpdateModel();
}

void Transform::SetPosition(const vec3 position)
{ 
	shouldUpdateTransform = true;
	this->position = { position };
}

void Transform::SetPosition(const float x, const float y, const float z)
{
	shouldUpdateTransform = true;
	this->position = { x, y, z };
}

void Transform::SetPosition(const float scalar)
{
	shouldUpdateTransform = true;
	this->position = { scalar, scalar, scalar };
}

void Transform::AddPosition(const vec3 position)
{
	shouldUpdateTransform = true;
	this->position = { this->position + position };
}

void Transform::AddPosition(const float x, const float y, const float z)
{
	shouldUpdateTransform = true;
	this->position = { (this->position) + (x, y, z) };
}

void Transform::AddPosition(const float scalar)
{
	shouldUpdateTransform = true;
	this->position = { this->position + vec3(scalar, scalar, scalar) };
}

void Transform::SetScale(const vec3 scale)
{
	shouldUpdateTransform = true;
	this->scale = { scale };
}

void Transform::SetScale(const float x, const float y, const float z)
{
	shouldUpdateTransform = true;
	this->scale = { x, y, z };
}

void Transform::SetScale(const float scale)
{
	shouldUpdateTransform = true;
	this->scale = { scale, scale, scale };
}

void Transform::AddScale(const vec3 scale)
{
	shouldUpdateTransform = true;
	this->scale = { this->scale + scale };
}

void Transform::AddScale(const float x, const float y, const float z)
{
	shouldUpdateTransform = true;
	this->scale = { this->scale + vec3(x, y, z) };
}

void Transform::AddScale(const float scale)
{
	shouldUpdateTransform = true;
	this->scale = { this->scale + vec3(scale, scale, scale) };
}

void Transform::SetRotation(const quat rotation)
{
	shouldUpdateTransform = true;
	this->rotation = { rotation };
}

void Transform::SetRotation(const float x, const float y, const float z)
{
	shouldUpdateTransform = true;
	this->rotation = glm::qua(glm::radians(vec3(x, y, z)));
}

void Transform::SetRotation(const vec3 euler)
{
	shouldUpdateTransform = true;
	this->rotation = glm::qua(glm::radians(euler)); 
}

void Transform::SetRotation(const float x, const float y, const float z, const float w)
{
	shouldUpdateTransform = true;
	this->rotation = { x, y, z, w };
}

void Transform::AddRotation(const quat rotation)
{
	shouldUpdateTransform = true;
	this->rotation = { this->rotation * rotation };
}

void Transform::AddRotation(const float x, const float y, const float z)
{
	shouldUpdateTransform = true;
	this->rotation = { this->rotation * glm::qua(glm::radians(vec3(x, y, z))) };
}

void Transform::AddRotation(const vec3 euler)
{
	shouldUpdateTransform = true;
	this->rotation = { this->rotation * glm::qua(glm::radians(euler)) };
}

void Transform::AddRotation(const float x, const float y, const float z, const float w)
{
	shouldUpdateTransform = true;
	this->rotation = { this->rotation * quat(x, y, z, w) };
}

void Transform::AddRotation(const float angle, const vec3 axis)
{
	shouldUpdateTransform = true;
	this->rotation = { this->rotation * glm::qua(glm::radians(angle), axis) };
}

void Transform::SetRotation(const float angle, const vec3 axis)
{
	shouldUpdateTransform = true;
	this->rotation = { glm::qua(glm::radians(angle), axis) };
}

void Transform::SetModelIndex(GLuint modelIndex)
{
	shouldUpdateTransform = true;
	this->modelIndex = modelIndex;
}

GLuint Transform::GetModelIndex()
{
	return modelIndex;
}

mat4 Transform::GetModelMatrix()
{
	return modelMatrix;
}

void Transform::UpdateModel()
{
	if (!shouldUpdateTransform) return;
	scaleMatrix = glm::scale(mat4(1.0f), scale);
	rotationMatrix = glm::mat4_cast(rotation);
	translation = glm::translate(mat4(1.0f), position);
	modelMatrix = translation * rotationMatrix * scaleMatrix;

	if (modelIndex == -1) return;
	outerLocation->at(batchIndex).modelMatrices.data()[modelIndex] = modelMatrix;
	shouldUpdateTransform = false;

}

void Transform::SetModelLocation(GLint modelIndex, GLint batchIndex)
{
	this->modelIndex = modelIndex;
	this->batchIndex = batchIndex;
}
