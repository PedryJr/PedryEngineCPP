#pragma once
#include "PedryEngine.h"


Shader::Shader()
{

    programId = 0;
    vertexArrayId = 0;
    vertexBuffer = 0;

	CompileShader();
	GenerateBuffers();
}

Shader::~Shader()
{
}

void Shader::GenerateBuffers()
{

    Shader::EnsureUseProgram(programId);

    glGenVertexArrays(1, &vertexArrayId);

    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &elementBuffer);
    glGenBuffers(1, &vertexModelsBuffer);

}

void Shader::UploadShape(GLfloat* positions, GLuint* indices, GLint points, GLint form)
{
	Shader::EnsureUseProgram(programId);
	glBindVertexArray(vertexArrayId);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * points, positions, GL_DYNAMIC_DRAW);
    GLint positionLocation = glGetAttribLocation(programId, "position");
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_TRUE, sizeof(GLfloat) * 3, (void*)0);
    glEnableVertexAttribArray(positionLocation);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * form, indices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, vertexModelsBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, INSTANCING_TEST * sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
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
	return vertexBuffer;
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

void Shader::CompileShader()
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
    char const* VertexSourcePointer = defaultVert;
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    char const* FragmentSourcePointer = defaultFrag;
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