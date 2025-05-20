#pragma once

#include "PedryEngine.h"

GLuint Shader::depthCubemap = -1;
GLuint Shader::depthMapFBO = -1;
GLuint64 Shader::shadowHandle = -1;

unsigned int Shader::SHADOW_WIDTH = 1024;
unsigned int Shader::SHADOW_HEIGHT = 1024;

Shader::~Shader()
{
}

void Shader::UploadShadowLight(bool withLightProjection) const
{

    if(withLightProjection)
    {
    
        Vector<mat4> shadowTransforms;
        shadowTransforms.push_back(Engine::lightProjection * glm::lookAt(Engine::lightPos, Engine::lightPos + vec3(1.0, 0.0, 0.0), vec3(0.0, -1.0, 0.0)));
        shadowTransforms.push_back(Engine::lightProjection * glm::lookAt(Engine::lightPos, Engine::lightPos + vec3(-1.0, 0.0, 0.0), vec3(0.0, -1.0, 0.0)));
        shadowTransforms.push_back(Engine::lightProjection * glm::lookAt(Engine::lightPos, Engine::lightPos + vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0)));
        shadowTransforms.push_back(Engine::lightProjection * glm::lookAt(Engine::lightPos, Engine::lightPos + vec3(0.0, -1.0, 0.0), vec3(0.0, 0.0, -1.0)));
        shadowTransforms.push_back(Engine::lightProjection * glm::lookAt(Engine::lightPos, Engine::lightPos + vec3(0.0, 0.0, 1.0), vec3(0.0, -1.0, 0.0)));
        shadowTransforms.push_back(Engine::lightProjection * glm::lookAt(Engine::lightPos, Engine::lightPos + vec3(0.0, 0.0, -1.0), vec3(0.0, -1.0, 0.0)));
        for (GLuint i = 0; i < 6; i++) SetMat4(shadowTransforms[i], "shadowMatrices[" + ToString(i) + "]");

    }
    else 
    {

        SetGLuint64(Shader::shadowHandle, "shadowMap");
        SetMat4(GlobalCamera::projectionMatrix * GlobalCamera::viewMatrix, "cameraMatrix");

    }

    SetVec3(Engine::lightPos, "lightPos");
    SetVec3(GlobalCamera::aimPosition, "camPos");
    SetFloat(Engine::lightFarPlane, "farPlane");

}

GLuint Shader::GetProgramID() const
{
	return programId;
}

Shader& Shader::GetShadowShader() const
{
    return *shadowShader;
}

GLuint Shader::GetModelMatrixBuffer() const
{
    return vertexModelsBuffer;
}

GLuint Shader::GetModelMainTextureBuffer() const
{
    return multiTextureBuffer;
}

GLuint Shader::GetModelHeightTextureBuffer() const
{
    return modelHeightTextureBuffer;
}

GLuint Shader::activeProgram = -1;
GLuint Shader::activeVAO = -1;
GLuint Shader::activeModelMatrixBuffer = -1;
GLuint Shader::activeModelMainTextureBuffer = -1;
GLuint Shader::activeModelHeightTextureBuffer = -1;
void Shader::EnsureUseProgram(GLuint pid)
{
	if (activeProgram != pid)
	{
		glUseProgram(pid);
		activeProgram = pid;
	}
}

void Shader::EnsureUseVAO(GLuint vao)
{
    if (activeVAO != vao)
    {
        glBindVertexArray(vao);
        activeVAO = vao;
    }
}

void Shader::EnsureUseModelMatrixBuffer(GLuint modelBuffer)
{
    if(activeModelMatrixBuffer != modelBuffer)
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, modelBuffer);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, modelBuffer);
		activeModelMatrixBuffer = modelBuffer;
    }
}

void Shader::EnsureUseModelMainTextureBuffer(GLuint modelBuffer)
{
    if (activeModelMainTextureBuffer != modelBuffer)
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, modelBuffer);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, modelBuffer);
        activeModelMainTextureBuffer = modelBuffer;
    }
}

void Shader::EnsureUseModelHeightTextureBuffer(GLuint modelBuffer)
{
    if (activeModelHeightTextureBuffer != modelBuffer)
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, modelBuffer);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, modelBuffer);
        activeModelHeightTextureBuffer = modelBuffer;
    }
}

void Shader::SetMat4(const mat4& data, const String& name) const
{
    glUniformMatrix4fv(glGetUniformLocation(programId, name.c_str()), 1, false, &data[0].x);
}
void Shader::SetInt(const GLint& data, const String& name) const
{
    glUniform1i(glGetUniformLocation(programId, name.c_str()), data);
}
void Shader::SetGLuint64(const GLuint64& data, const String& name) const
{
    glUniform1ui64ARB(glGetUniformLocation(programId, name.c_str()), data);
}
void Shader::SetFloat(const GLfloat& data, const String& name) const
{
    glUniform1f(glGetUniformLocation(programId, name.c_str()), data);
}
void Shader::SetVec2(const vec2& data, const String& name) const
{
    glUniform2f(glGetUniformLocation(programId, name.c_str()), data.x, data.y);
}

void Shader::SetVec3(const vec3& data, const String& name) const
{
    glUniform3f(glGetUniformLocation(programId, name.c_str()), data.x, data.y, data.z);
}

void Shader::SetVec4(const vec4& data, const String& name) const
{
    glUniform4f(glGetUniformLocation(programId, name.c_str()), data.x, data.y, data.z, data.w);
}

void CheckShaderError(const GLuint shaderID, const GLenum type) {
    GLint success = 0;
    if (type == GL_COMPILE_STATUS) {
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[1024];
            glGetShaderInfoLog(shaderID, sizeof(infoLog), NULL, infoLog);
            std::cerr << "Shader Compile Error:\n" << infoLog << std::endl;
        }
    }
    else if (type == GL_LINK_STATUS) {
        glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[1024];
            glGetProgramInfoLog(shaderID, sizeof(infoLog), NULL, infoLog);
            std::cerr << "Program Link Error:\n" << infoLog << std::endl;
        }
    }
}

Shader::Shader(const String name, const GLulong shaderId)
{

	this->shaderId = shaderId;
	this->shadowShader = nullptr;
    
    bool vert, geom, tesc, tese, frag;
    programId = glCreateProgram();
    
    GLuint VertexShaderID;
    GLuint GeometryShaderID;
    GLuint TessellationCShaderID;
    GLuint TessellationEShaderID;
    GLuint FragmentShaderID;

    String vertCode;
    String geomCode;
    String tescCode;
    String teseCode;
    String fragCode;

    ReadFileToString("Assets/" + name + ".vert", vertCode, vert);
    ReadFileToString("Assets/" + name + ".geom", geomCode, geom);
    ReadFileToString("Assets/" + name + ".tesc", tescCode, tesc);
    ReadFileToString("Assets/" + name + ".tese", teseCode, tese);
    ReadFileToString("Assets/" + name + ".frag", fragCode, frag);

    if (vert) 
    {
        char const* VertexSourcePointer = vertCode.c_str();
        VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
        glCompileShader(VertexShaderID);
        CheckShaderError(VertexShaderID, GL_COMPILE_STATUS);
        glAttachShader(programId, VertexShaderID);
    }
    if (geom)
    {
        char const* GeometrySourcePointer = geomCode.c_str();
        GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(GeometryShaderID, 1, &GeometrySourcePointer, NULL);
        glCompileShader(GeometryShaderID);
        CheckShaderError(GeometryShaderID, GL_COMPILE_STATUS);
        glAttachShader(programId, GeometryShaderID);
    }
    if (tesc)
    {
        char const* TesselationControlSourcePointer = tescCode.c_str();
        TessellationCShaderID = glCreateShader(GL_TESS_CONTROL_SHADER);
        glShaderSource(TessellationCShaderID, 1, &TesselationControlSourcePointer, NULL);
        glCompileShader(TessellationCShaderID);
        CheckShaderError(TessellationCShaderID, GL_COMPILE_STATUS);
        glAttachShader(programId, TessellationCShaderID);
    }
    if (tese)
    {
        char const* TesselationEvaluationSourcePointer = teseCode.c_str();
        TessellationEShaderID = glCreateShader(GL_TESS_EVALUATION_SHADER);
        glShaderSource(TessellationEShaderID, 1, &TesselationEvaluationSourcePointer, NULL);
        glCompileShader(TessellationEShaderID);
        CheckShaderError(TessellationEShaderID, GL_COMPILE_STATUS);
        glAttachShader(programId, TessellationEShaderID);
    }
    if (frag)
    {
        char const* FragmentSourcePointer = fragCode.c_str();
        FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
        glCompileShader(FragmentShaderID);
        CheckShaderError(FragmentShaderID, GL_COMPILE_STATUS);
        glAttachShader(programId, FragmentShaderID);
    }
    glLinkProgram(programId);
    CheckShaderError(programId, GL_LINK_STATUS);
    if (vert)
    {
        glDetachShader(programId, VertexShaderID);
        glDeleteShader(VertexShaderID);
    }
    if (geom)
    {
        glDetachShader(programId, GeometryShaderID);
        glDeleteShader(GeometryShaderID);
    }
    if (tesc)
    {
        glDetachShader(programId, TessellationCShaderID);
        glDeleteShader(TessellationCShaderID);
    }
    if (tese)
    {
        glDetachShader(programId, TessellationEShaderID);
        glDeleteShader(TessellationEShaderID);
    }
    if (frag)
    {
        glDetachShader(programId, FragmentShaderID);
        glDeleteShader(FragmentShaderID);
    }

    glGenBuffers(1, &vertexModelsBuffer);
    glGenBuffers(1, &multiTextureBuffer);
    glGenBuffers(1, &modelHeightTextureBuffer);

    std::cout << "MODEL BUFFER = " << vertexModelsBuffer << std::endl;

    if (depthCubemap == -1)
    {

        glGenTextures(1, &depthCubemap);
        glGenFramebuffers(1, &depthMapFBO);

        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
        for (unsigned int i = 0; i < 6; ++i) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    }

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //Vector<GLuint64> textureHandles;
    //textureHandles.push_back(AssetManager::LoadTexture("default"));

    //glBindBuffer(GL_SHADER_STORAGE_BUFFER, multiTextureBuffer);
    //glNamedBufferStorage(
    //    multiTextureBuffer,
    //    sizeof(GLuint64) * textureHandles.size(),
    //    (const void*)textureHandles.data(),
    //    GL_DYNAMIC_STORAGE_BIT
    //);

    //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, multiTextureBuffer);

}

