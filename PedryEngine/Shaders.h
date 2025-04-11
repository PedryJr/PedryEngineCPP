#pragma once
#ifndef Shaders
#define Shaders
#define defaultVert R"V0G0N(
#version 430 core

precision highp float;
precision highp int;

layout(location = 0) in vec3 position;

layout(std430, binding = 1) buffer Matrices {
    vec4 modelMatrices[]; // Storage for matrices
};

uniform vec2 aspect;

out vec4 color;
out vec3 FragPos;

void main()
{

    vec3 newPos = vec3(vec3(position)*modelMatrices[gl_InstanceID].w);
    vec2 model = vec2(modelMatrices[gl_InstanceID]);
    gl_Position = (vec4(newPos, 1.0) + vec4(model, 0, 0)) / vec4(5.0, 5.0, 5.0, 1.0);
    color = vec4(0.25, 0.25, 0.25 / 0.25, 0.5);
    FragPos = vec3(gl_Position);

}
)V0G0N"

#define defaultFrag R"V0G0N(
#version 430 core

in vec4 color;
out vec4 FragColor;

void main()
{

    FragColor = color;

}
)V0G0N"
#endif
//vec3 modelMatrix = modelMatrices[gl_InstanceID];
//
//vec3 modifiedPosition = position / modelMatrix.z);
//vec2 offset = vec2(modelMatrix) / vec2(10.0);
//gl_Position = vec4(vec2(modifiedPosition) + offset, 0, 1.0);
//color = vec4(0.25, 0.25, 0.25 / 0.25, 0.1);