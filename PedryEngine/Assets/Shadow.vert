#version 460

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUv;

layout(std430, binding = 1) buffer Matrices {
    mat4 modelMatrices[];
};


void main() 
{
    gl_Position = modelMatrices[gl_InstanceID] * vec4(vertexPosition, 1.0);
}