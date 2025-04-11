#pragma once
#ifndef PengShader
#define PengShader
class Shader
{
public:
	Shader();
	~Shader();
	//Exposed Functions
	void UploadShape(GLfloat* positions, GLuint* indices, GLint points, GLint form);

	GLuint GetProgramID();
	GLuint GetVertexArrayID();
	GLuint GetVertexBuffer();
	GLuint GetModelBuffer();

	static void EnsureUseProgram(GLuint programId);

private:
	//Inner Functions
	void CompileShader();
	void GenerateBuffers();

	GLuint programId;

	GLuint vertexArrayId;
	
	GLuint vertexBuffer;
	GLuint elementBuffer;

	GLuint vertexModelsBuffer;

	static GLuint activeProgram;

};
#endif