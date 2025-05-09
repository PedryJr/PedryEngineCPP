#pragma once
class VisualShape
{
public:
	VisualShape();
	~VisualShape();

	void ApplyDefault();

	Vector<GLfloat> GetPositions();
	Vector<GLuint> GetIndices();
	GLint GetPoints();
	GLint GetForm();

	Shader* GetShader();


private:

	Shader* shader;

	mat4 model;

	Vector<GLfloat> positions;
	Vector<GLuint> indices;
	GLint points;
	GLint form;

	void AllocateDefaultShape();

};