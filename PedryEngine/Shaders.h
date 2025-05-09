#pragma once
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

precision highp float;
precision highp int;

in vec4 color;
out vec4 FragColor;

void main()
{

    FragColor = color;

}
)V0G0N"

#define Vert3D R"V0G0N(
#version 430 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUv;
layout(location = 3) in vec4 vertexTangent;

layout(std430, binding = 1) buffer Matrices {
    mat4 modelMatrices[]; // Storage for matrices
};

out vec3 normalOut;
out vec3 FragPos;

uniform mat4 rotationMatrix;
uniform mat4 cameraMatrix;

void main()
{

    mat4 modelMatrix = modelMatrices[gl_InstanceID];
    mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));

    gl_Position = cameraMatrix * modelMatrix * vec4(vertexPosition, 1.0);
    normalOut = normalMatrix * vertexNormal;
    FragPos = vec3(modelMatrix * vec4(vertexPosition, 1.0));

}
)V0G0N"

#define Frag3D R"V0G0N(
#version 430 core

in vec3 normalOut;
in vec3 FragPos;

uniform vec3 lightPos;

out vec4 FragColor;

void main()
{

    vec3 lightColor = vec3(1.0, 1.0, 1.0);

    vec3 norm = normalize(normalOut);
    vec3 lightDir = normalize(lightPos - FragPos);  

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    FragColor = vec4(diffuse, 1.0);

    vec4 lmao = vec4(normalOut, 1.0);

    lmao.x = abs(lmao.x);
    lmao.y = abs(lmao.y);
    lmao.z = abs(lmao.z);
    lmao.w = abs(lmao.w);

    FragColor = lmao;

}
)V0G0N"

//vec3 modelMatrix = modelMatrices[gl_InstanceID];
//
//vec3 modifiedPosition = position / modelMatrix.z);
//vec2 offset = vec2(modelMatrix) / vec2(10.0);
//gl_Position = vec4(vec2(modifiedPosition) + offset, 0, 1.0);
//color = vec4(0.25, 0.25, 0.25 / 0.25, 0.1);