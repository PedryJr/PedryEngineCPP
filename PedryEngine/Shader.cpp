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

    Shader::EnsureUseProgram(programId);

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

	Shader::EnsureUseProgram(programId);
	glBindVertexArray(vertexArrayId);

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


    glBindBuffer(GL_ARRAY_BUFFER, tangentBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * tangentsCount, tangents, GL_STATIC_COPY);
    GLint tangentLocation = glGetAttribLocation(programId, "vertexTangent");
    glVertexAttribPointer(tangentLocation, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (void*)0);
    glEnableVertexAttribArray(tangentLocation);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indiciesCount, indices, GL_STATIC_COPY);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, vertexModelsBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 0, NULL, GL_STATIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vertexModelsBuffer);

    Vector<GLuint64> textureHandles;

    GLuint64 handle = glGetTextureHandleARB(AssetManager::LoadTexture("default"));
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


}

GLuint Shader::GetProgramID()
{
	return programId;
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
void Shader::EnsureUseProgram(GLuint programId)
{
	if (activeProgram != programId)
	{
		glUseProgram(programId);
		activeProgram = programId;
	}
}

void Shader::SetMat4(const mat4& data, const String& name)
{
    EnsureUseProgram(programId);
    glUniformMatrix4fv(glGetUniformLocation(programId, name.c_str()), 1, false, &data[0].x);
}

void Shader::SetVec2(const vec2& data, const String& name)
{
    glUniform2fv(glGetUniformLocation(programId, name.c_str()), false, &data.x);
}

void Shader::SetVec3(const vec3& data, const String& name)
{
    glUniform2fv(glGetUniformLocation(programId, name.c_str()), false, &data.x);
}

void Shader::SetVec4(const vec4& data, const String& name)
{
    glUniform2fv(glGetUniformLocation(programId, name.c_str()), false, &data.x);
}

void Shader::CompileShader(String vert, String frag)
{
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    if (VertexShaderID == 0) {
        printf("Error creating vertex shader\n");
        return;
    }

    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    if (FragmentShaderID == 0) {
        printf("Error creating fragment shader\n");
        return;
    }


    printf("Vertex shader created\n");
    char const* VertexSourcePointer = vert.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    char const* FragmentSourcePointer = frag.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);
    printf("Fragment shader created\n");

    // Link the program
    printf("Linking program\n");
    programId = glCreateProgram();
    glAttachShader(programId, VertexShaderID);
    glAttachShader(programId, FragmentShaderID);
    glLinkProgram(programId);

    glDetachShader(programId, VertexShaderID);
    glDetachShader(programId, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

}



//int vertexFileSize;
//Stat fileStats;
//FILE* shaderFile;
//Vector<GLchar> unrolled;
//
//vertexFileSize = 0;
//
//shaderFile = fopen("dShader.vert", "rb");
//fstat(fileno(shaderFile), &fileStats);
//unrolled = Vector<GLchar>(fileStats.st_size);
//fread(&unrolled[0], sizeof(GLchar), fileStats.st_size, shaderFile);