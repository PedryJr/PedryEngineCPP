#pragma once

#include "PedryEngine.h"

Shader::Shader()
{

    programId = 0;
    vertexArrayId = 0;
    positionBuffer = 0;

	CompileShader();
	GenerateBuffers();

}

Shader::Shader(String vert, String frag)
{

    programId = 0;
    vertexArrayId = 0;
    positionBuffer = 0;

    CompileShader(vert, frag);
    GenerateBuffers();

}

Shader::~Shader()
{
}

void Shader::GenerateBuffers()
{

    Shader::EnsureUseProgram(programId, vertexArrayId);

    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    glGenBuffers(1, &positionBuffer);
    glGenBuffers(1, &normalBuffer);
    glGenBuffers(1, &uvBuffer);
    glGenBuffers(1, &tangentBuffer);
    glGenBuffers(1, &elementBuffer);
    glGenBuffers(1, &vertexModelsBuffer);
    glGenBuffers(1, &multiTextureBuffer);

    std::cout << "MODEL BUFFER = " << vertexModelsBuffer << std::endl;

}

void Shader::UploadMesh(Mesh* mesh)
{

    GLfloat* positions = mesh->vertices.data();
    GLuint* indices = mesh->indices.data();
    GLfloat* normals = mesh->normals.data();
    GLfloat* uvs = mesh->uvs.data();
    GLfloat* tangents = mesh->tangents.data();

    GLint positionsCount = mesh->vertices.size();
    GLint indiciesCount = mesh->indices.size();
    GLint normalsCount = mesh->normals.size();
    GLint uvsCount = mesh->uvs.size();
    GLint tangentsCount = mesh->tangents.size();

    std::cout << positionsCount << programId << std::endl;
    std::cout << indiciesCount << programId << std::endl;

	Shader::EnsureUseProgram(programId, vertexArrayId);


	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * positionsCount, positions, GL_STATIC_COPY);
    GLint positionLocation = glGetAttribLocation(programId, "vertexPosition");
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void*)0);
    glEnableVertexAttribArray(positionLocation);


    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * normalsCount, normals, GL_STATIC_COPY);
    GLint normalLocation = glGetAttribLocation(programId, "vertexNormal");
    glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void*)0);
    glEnableVertexAttribArray(normalLocation);


    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * uvsCount, uvs, GL_STATIC_COPY);
    GLint uvLocation = glGetAttribLocation(programId, "vertexUv");
    glVertexAttribPointer(uvLocation, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (void*)0);
    glEnableVertexAttribArray(uvLocation);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indiciesCount, indices, GL_STATIC_COPY);


    glBindBuffer(GL_SHADER_STORAGE_BUFFER, vertexModelsBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 0, NULL, GL_STATIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vertexModelsBuffer);

    //Template texture code
    Vector<GLuint64> textureHandles;

    GLuint64 handle = glGetTextureHandleARB(AssetManager::LoadTexture("megumin"));
    glMakeTextureHandleResidentARB(handle);
    textureHandles.push_back(handle);

    std::cout << "LOOL" << handle << std::endl;

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, multiTextureBuffer);
    glNamedBufferStorage(
        multiTextureBuffer,
        sizeof(GLuint64) * textureHandles.size(),
        (const void*)textureHandles.data(),
        GL_DYNAMIC_STORAGE_BIT
    );
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, multiTextureBuffer);
    //

    //Shader::EnsureUseProgram(shadowId, vertexArrayId);
    //glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    //glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void*)0);
    //glEnableVertexAttribArray(positionLocation);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indiciesCount, indices, GL_STATIC_COPY);

}

void Shader::UploadShadowLight()
{

	Shader::EnsureUseProgram(shadowId, vertexArrayId);

	vec3 lightPos = Engine::lightPos;
	mat4 lightProjection = Engine::lightProjection;
    Vector<mat4> shadowTransforms;
    shadowTransforms.push_back(lightProjection * glm::lookAt(lightPos, lightPos + vec3(1.0, 0.0, 0.0), vec3(0.0, -1.0, 0.0)));
    shadowTransforms.push_back(lightProjection * glm::lookAt(lightPos, lightPos + vec3(-1.0, 0.0, 0.0), vec3(0.0, -1.0, 0.0)));
    shadowTransforms.push_back(lightProjection * glm::lookAt(lightPos, lightPos + vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0)));
    shadowTransforms.push_back(lightProjection * glm::lookAt(lightPos, lightPos + vec3(0.0, -1.0, 0.0), vec3(0.0, 0.0, -1.0)));
    shadowTransforms.push_back(lightProjection * glm::lookAt(lightPos, lightPos + vec3(0.0, 0.0, 1.0), vec3(0.0, -1.0, 0.0)));
    shadowTransforms.push_back(lightProjection * glm::lookAt(lightPos, lightPos + vec3(0.0, 0.0, -1.0), vec3(0.0, -1.0, 0.0)));

	SetVec3(lightPos, "lightPos");
    SetFloat(Engine::lightFarPlane, "farPlane");
    for(GLuint i = 0; i < 6; i++) SetMat4(shadowTransforms[i], "shadowMatrices[" + ToString(i) + "]");

}

GLuint Shader::GetProgramID()
{
	return programId;
}

GLuint Shader::GetShadowID()
{
    return shadowId;
}

GLuint Shader::GetVertexArrayID()
{
	return vertexArrayId;
}

GLuint Shader::GetVertexBuffer()
{
	return positionBuffer;
}

GLuint Shader::GetModelBuffer()
{
    return vertexModelsBuffer;
}

GLuint Shader::activeProgram = -1;
void Shader::EnsureUseProgram(GLuint programId, GLuint vao)
{
	if (activeProgram != programId)
	{
		glUseProgram(programId);
        glBindVertexArray(vao);
		activeProgram = programId;
	}
}

void Shader::SetMat4(const mat4& data, const String& name)
{
    EnsureUseProgram(shadowId, vertexArrayId);
    glUniformMatrix4fv(glGetUniformLocation(shadowId, name.c_str()), 1, false, &data[0].x);
    EnsureUseProgram(programId, vertexArrayId);
    glUniformMatrix4fv(glGetUniformLocation(programId, name.c_str()), 1, false, &data[0].x);
}
void Shader::SetInt(const GLint& data, const String& name)
{
    EnsureUseProgram(shadowId, vertexArrayId);
    glUniform1i(glGetUniformLocation(shadowId, name.c_str()), data);
    EnsureUseProgram(programId, vertexArrayId);
    glUniform1i(glGetUniformLocation(programId, name.c_str()), data);
}
void Shader::SetGLuint64(const GLuint64& data, const String& name)
{
    EnsureUseProgram(shadowId, vertexArrayId);
    glUniform1ui64ARB(glGetUniformLocation(shadowId, name.c_str()), data);
    EnsureUseProgram(programId, vertexArrayId);
    glUniform1ui64ARB(glGetUniformLocation(programId, name.c_str()), data);
}
void Shader::SetFloat(const GLfloat& data, const String& name)
{
    EnsureUseProgram(shadowId, vertexArrayId);
    glUniform1f(glGetUniformLocation(shadowId, name.c_str()), data);
    EnsureUseProgram(programId, vertexArrayId);
    glUniform1f(glGetUniformLocation(programId, name.c_str()), data);
}
void Shader::SetVec2(const vec2& data, const String& name)
{
	EnsureUseProgram(shadowId, vertexArrayId);
    glUniform2f(glGetUniformLocation(shadowId, name.c_str()), data.x, data.y);
	EnsureUseProgram(programId, vertexArrayId);
    glUniform2f(glGetUniformLocation(programId, name.c_str()), data.x, data.y);
}

void Shader::SetVec3(const vec3& data, const String& name)
{
	EnsureUseProgram(shadowId, vertexArrayId);
    glUniform3f(glGetUniformLocation(shadowId, name.c_str()), data.x, data.y, data.z);
	EnsureUseProgram(programId, vertexArrayId);
    glUniform3f(glGetUniformLocation(programId, name.c_str()), data.x, data.y, data.z);
}

void Shader::SetVec4(const vec4& data, const String& name)
{
    EnsureUseProgram(shadowId, vertexArrayId);
    glUniform4f(glGetUniformLocation(shadowId, name.c_str()), data.x, data.y, data.z, data.w);
    EnsureUseProgram(programId, vertexArrayId);
    glUniform4f(glGetUniformLocation(programId, name.c_str()), data.x, data.y, data.z, data.w);
}

// Helper function to print compile or link errors
void CheckShaderError(GLuint shaderID, GLenum type) {
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


void Shader::CompileShader(String vert, String frag)
{

    GLuint VertexShaderID;
    GLuint GeometryShaderID;
    GLuint FragmentShaderID;

    char const* VertexSourcePointer;
    char const* GeometrySourcePointer;
    char const* FragmentSourcePointer;

    // Light shaders
    VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    VertexSourcePointer = vert.c_str();
    FragmentSourcePointer = frag.c_str();

    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    programId = glCreateProgram();

    glAttachShader(programId, VertexShaderID);
    glAttachShader(programId, FragmentShaderID);

    glLinkProgram(programId);

    glDetachShader(programId, VertexShaderID);
    glDetachShader(programId, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    //

	String shadVert, shadGeom, shadFrag;
	shadVert = VertShadow;
	shadGeom = GeomShadow;
    shadFrag = FragShadow;

	// Shadow shaders
    VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
    FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    VertexSourcePointer = shadVert.c_str();
    GeometrySourcePointer = shadGeom.c_str();
    FragmentSourcePointer = shadFrag.c_str();

    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);
    CheckShaderError(VertexShaderID, GL_COMPILE_STATUS);

    glShaderSource(GeometryShaderID, 1, &GeometrySourcePointer, NULL);
    glCompileShader(GeometryShaderID);
    CheckShaderError(GeometryShaderID, GL_COMPILE_STATUS);

    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);
    CheckShaderError(FragmentShaderID, GL_COMPILE_STATUS);

    shadowId = glCreateProgram();

    glAttachShader(shadowId, VertexShaderID);
    glAttachShader(shadowId, GeometryShaderID);
    glAttachShader(shadowId, FragmentShaderID);

    glLinkProgram(shadowId);
    CheckShaderError(shadowId, GL_LINK_STATUS);

    glDetachShader(shadowId, VertexShaderID);
    glDetachShader(shadowId, GeometryShaderID);
    glDetachShader(shadowId, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(GeometryShaderID);
    glDeleteShader(FragmentShaderID);
	//

    // Generate cubemap
    glGenTextures(1, &depthCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    for (unsigned int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
            SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }
    //

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Attach to FBO
    glGenFramebuffers(1, &depthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //
}