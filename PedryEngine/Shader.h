#pragma once
class Shader
{
public:
	Shader();
	Shader(String vert, String frag);
	~Shader();

	//Exposed Functions
	void UploadMesh(Mesh* mesh);
	void UploadShadowLight();

	GLuint GetProgramID();
	GLuint GetShadowID();
	GLuint GetVertexArrayID();
	GLuint GetVertexBuffer();
	GLuint GetModelBuffer();

	static void EnsureUseProgram(GLuint programId, GLuint vao);

	void SetMat4(const mat4& data, const String& name);
	void SetInt(const GLint& data, const String& name);
	void SetGLuint64(const GLuint64& data, const String& name);
	void SetFloat(const GLfloat& data, const String& name);
	void SetVec2(const vec2& data, const String& name);
	void SetVec3(const vec3& data, const String& name);
	void SetVec4(const vec4& data, const String& name);

	const unsigned int SHADOW_WIDTH = 1024 * 2, SHADOW_HEIGHT = 1024 * 2;
	GLuint depthMapFBO;
	GLuint depthCubemap;

private:
	//Inner Functions
	void CompileShader(String vert = Vert3D, String frag = Frag3D);
	void GenerateBuffers();

	GLuint programId;

	GLuint vertexArrayId;
	
	GLuint positionBuffer;
	GLuint normalBuffer;
	GLuint uvBuffer;
	GLuint tangentBuffer;

	GLuint elementBuffer;

	GLuint vertexModelsBuffer;

	GLuint multiTextureBuffer;

	static GLuint activeProgram;

	GLuint shadowId;

public:
	Vector<mat4> shadowTransforms;

};