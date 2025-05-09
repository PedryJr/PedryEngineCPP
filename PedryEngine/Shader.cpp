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

    glBindVertexArray(-1);
    Shader::EnsureUseProgram(programId);

    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    glGenBuffers(1, &positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);

    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);

    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);

    glGenBuffers(1, &tangentBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, tangentBuffer);

    glGenBuffers(1, &elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

    glGenBuffers(1, &vertexModelsBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, vertexModelsBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vertexModelsBuffer);

    std::cout << "MODEL BUFFER = " << vertexModelsBuffer << std::endl;

}

void Shader::UploadShape(GLfloat* positions, GLuint* indices, GLfloat* normals, GLfloat* uvs, GLfloat* tangents, GLint points, GLint form)
{
	Shader::EnsureUseProgram(programId);
	glBindVertexArray(vertexArrayId);

	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * points, positions, GL_DYNAMIC_DRAW);
    GLint positionLocation = glGetAttribLocation(programId, "vertexPosition");
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void*)0);
    glEnableVertexAttribArray(positionLocation);

    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * points, normals, GL_DYNAMIC_DRAW);
    GLint normalLocation = glGetAttribLocation(programId, "vertexNormal");
    glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void*)0);
    glEnableVertexAttribArray(normalLocation);

    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * points, uvs, GL_DYNAMIC_DRAW);
    GLint uvLocation = glGetAttribLocation(programId, "vertexUv");
    glVertexAttribPointer(uvLocation, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (void*)0);
    glEnableVertexAttribArray(uvLocation);

    glBindBuffer(GL_ARRAY_BUFFER, tangentBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * points, tangents, GL_DYNAMIC_DRAW);
    GLint tangentLocation = glGetAttribLocation(programId, "vertexTangent");
    glVertexAttribPointer(tangentLocation, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (void*)0);
    glEnableVertexAttribArray(tangentLocation);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * form, indices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, vertexModelsBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vertexModelsBuffer);

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