#pragma once
#include "PedryEngine.h"
VisualShape::VisualShape()
{
	shader = new Shader();
	AllocateDefaultShape();
	printf("%d%s", shader->GetProgramID(), "\n");
	printf("%d%s", shader->GetVertexArrayID(), "\n");
	printf("%d%s", shader->GetVertexBuffer(), "\n");
}


VisualShape::~VisualShape()
{
	//free(positions);
	delete shader;
}

void VisualShape::ApplyDefault()
{
	//free(positions);
	AllocateDefaultShape();
}

Vector<GLfloat> VisualShape::GetPositions()
{
	return positions;
}

Vector<GLuint> VisualShape::GetIndices()
{
	return indices;
}

GLint VisualShape::GetPoints()
{
	return points;
}

GLint VisualShape::GetForm()
{
	return form;
}

Shader* VisualShape::GetShader()
{
	return shader;
}

void VisualShape::AllocateDefaultShape()
{
	GLfloat defaultShape[DEFAULT_SHAPE_COUNT] =
	{
		-0.4, 0.0, 0.0,
		0.4, 0.0, 0.0,
		0.0, 1.0, 0.0,
	};


	positions = Vector<GLfloat>(DEFAULT_SHAPE_COUNT);
	for (int i = 0; i < DEFAULT_SHAPE_COUNT; i++) positions.at(i) = (defaultShape[i]);
	points = DEFAULT_SHAPE_COUNT;

	GLuint defaultForm[DEFAULT_FORM_COUNT] = {
	0, 1, 2,
	};

	indices = Vector<GLuint>(DEFAULT_FORM_COUNT);
	for (int i = 0; i < DEFAULT_FORM_COUNT; i++) indices.at(i) = (defaultForm[i]);
	form = DEFAULT_FORM_COUNT;

	shader->UploadShape(positions.data(), indices.data(), points, form);
}
