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
out vec2 FragCoord;

uniform mat4 rotationMatrix;
uniform mat4 cameraMatrix;

void main()
{

    mat4 modelMatrix = modelMatrices[gl_InstanceID];
    mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));

    gl_Position = cameraMatrix * modelMatrix * vec4(vertexPosition, 1.0);
    normalOut = normalMatrix * vertexNormal;

    FragPos = vec3(modelMatrix * vec4(vertexPosition, 1.0));
    FragCoord = vertexUv;

}
)V0G0N"

#define Frag3D R"V0G0N(
#version 430 core

#extension GL_ARB_bindless_texture : require


layout(binding = 2, std430) readonly buffer ssbo3 {
    sampler2D textures[];
};


in vec3 normalOut;
in vec3 FragPos;
in vec2 FragCoord;

uniform vec3 lightPos;
uniform vec3 camPos;

out vec4 FragColor;

float specularStrength = 1;

void main()
{

    sampler2D textureSampler = textures[0];
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    float lightDistance = 400;
    float distanceToLight = distance(FragPos, lightPos);
    lightColor = lightColor / (distanceToLight / lightDistance);

    lightColor = clamp(lightColor, 0, 1);

    vec4 texColor = texture(textureSampler, FragCoord);

    vec3 norm = normalize(normalOut);
    vec3 lightDir = normalize(lightPos - FragPos);

    vec3 viewDir = normalize(camPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 40);
    vec3 specular = specularStrength * spec * lightColor;

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 result = (diffuse + specular) * vec3(texColor);
    FragColor = vec4(result, 1.0);


}
)V0G0N"

//vec3 modelMatrix = modelMatrices[gl_InstanceID];
//
//vec3 modifiedPosition = position / modelMatrix.z);
//vec2 offset = vec2(modelMatrix) / vec2(10.0);
//gl_Position = vec4(vec2(modifiedPosition) + offset, 0, 1.0);
//color = vec4(0.25, 0.25, 0.25 / 0.25, 0.1);