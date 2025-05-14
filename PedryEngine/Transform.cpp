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

void Transform::SetPosition(vec3 position)
{ this->position = position; }

void Transform::SetScale(vec3 scale)
{ this->scale = scale; }

void Transform::SetRotation(vec3 euler)
{ this->rotation = glm::qua(glm::radians(euler)); }

void Transform::SetModelIndex(GLuint modelIndex)
{
	this->modelIndex = modelIndex;
}

GLuint Transform::GetModelIndex()
{
	return modelIndex;
}

mat4 Transform::GetModelMatrix()
{
	mat4 scaleMatrix = glm::transpose(glm::scale(mat4(1.0f), scale));
	mat4 rotationMatrix = glm::mat4_cast(rotation);
	mat4 translation = glm::translate(mat4(1.0f), position);
	return translation * rotationMatrix * scaleMatrix;
}

void Transform::UpdateModel()
{

	if (assignedBatch == nullptr) return;

	mat4 scaleMatrix = glm::transpose(glm::scale(mat4(1.0f), scale));
	mat4 rotationMatrix = glm::mat4_cast(rotation);
	mat4 translation = glm::translate(mat4(1.0f), position);
	mat4 modelMatrix = translation * rotationMatrix * scaleMatrix;

	mat4 view = GlobalCamera::GetViewMatrix();

	mat4 Projection = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.01f, 100.0f);

	assignedBatch->modelMatrices.data()[modelIndex] = modelMatrix;
	assignedBatch->shader->SetMat4(rotationMatrix, "rotationMatrix");
	assignedBatch->shader->SetMat4(Projection * view, "cameraMatrix");
	assignedBatch->shader->SetVec3(vec3(0.0F, 0.0F, 0.0F), "lightPos");
	assignedBatch->shader->SetVec3(GlobalCamera::aimPosition, "camPos");
}

void Transform::SetModelLocation(DrawCallBatch& assignedBatch, GLint index)
{
	this->assignedBatch = &assignedBatch;
	modelIndex = index;
}
