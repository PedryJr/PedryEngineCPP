#pragma once
class Shader
{
public:
	Shader();
	Shader(String vert, String frag);
	~Shader();

	//Exposed Functions
	void UploadMesh(Mesh* mesh);

	GLuint GetProgramID();
	GLuint GetVertexArrayID();
	GLuint GetVertexBuffer();
	GLuint GetModelBuffer();

	static void EnsureUseProgram(GLuint programId);

	void SetMat4(const mat4& data, const String& name);
	void SetVec2(const vec2& data, const String& name);
	void SetVec3(const vec3& data, const String& name);
	void SetVec4(const vec4& data, const String& name);

private:
	//Inner Functions
	void CompileShader(String vert = defaultVert, String frag = defaultFrag);
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

};