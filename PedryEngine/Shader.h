#pragma once
class Shader
{

public:
	Shader(const String name, const GLulong shaderId);
	~Shader();

	//Exposed Functions
	void UploadShadowLight(bool withLightProjection = true) const;

	GLuint GetProgramID() const;
	Shader& GetShadowShader() const;

	GLuint GetModelMatrixBuffer() const;
	GLuint GetModelMainTextureBuffer() const;
	GLuint GetModelHeightTextureBuffer() const;

	static void EnsureUseProgram(GLuint programId);
	static void EnsureUseVAO(GLuint vao);
	static void EnsureUseModelMatrixBuffer(GLuint modelBuffer);
	static void EnsureUseModelMainTextureBuffer(GLuint modelBuffer);
	static void EnsureUseModelHeightTextureBuffer(GLuint modelBuffer);

	void SetMat4(const mat4& data, const String& name) const;
	void SetInt(const GLint& data, const String& name) const;
	void SetGLuint64(const GLuint64& data, const String& name) const;
	void SetFloat(const GLfloat& data, const String& name) const;
	void SetVec2(const vec2& data, const String& name) const;
	void SetVec3(const vec3& data, const String& name) const;
	void SetVec4(const vec4& data, const String& name) const;

	static unsigned int SHADOW_WIDTH, SHADOW_HEIGHT;
	static GLuint depthMapFBO;
	static GLuint depthCubemap;
	static GLuint64 shadowHandle;

	Shader* shadowShader;
	GLulong shaderId;

private:

	GLuint programId;

	GLuint vertexModelsBuffer;
	GLuint multiTextureBuffer;
	GLuint modelHeightTextureBuffer;

	static GLuint activeProgram;
	static GLuint activeVAO;
	static GLuint activeModelMatrixBuffer;
	static GLuint activeModelMainTextureBuffer;
	static GLuint activeModelHeightTextureBuffer;

	//GLuint shadowId;

public:
	Vector<mat4> shadowTransforms;

};