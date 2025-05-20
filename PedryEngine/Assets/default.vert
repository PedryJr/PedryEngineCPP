#version 460
precision highp float;
precision highp int;
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUv;

layout(binding = 1) buffer Matrices {
    mat4 modelMatrices[]; // Storage for matrices
};

uniform mat4 rotationMatrix;
uniform mat4 cameraMatrix;

out vec3 normalOut;
out vec3 FragPos;
out vec2 FragCoord;
out int instanceId;

void main()
{
    instanceId = gl_InstanceID;

    mat4 modelMatrix = modelMatrices[gl_InstanceID];
    mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));

    gl_Position = cameraMatrix * modelMatrix * vec4(vertexPosition, 1.0);
    normalOut = normalMatrix * vertexNormal;

    FragPos = vec3(modelMatrix * vec4(vertexPosition, 1.0));
    FragCoord = vertexUv;
}