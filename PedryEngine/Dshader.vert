#version 430 core

layout(location = 0) in vec3 position;

layout(std430, binding = 1) buffer Matrices {
vec3 matrices[]; // Storage for matrices
};

out float instanceID;

void main()
{
    vec3 modifiedPosition = position / vec3(matrices[gl_InstanceID].z);
    vec2 offset = vec2(matrices[gl_InstanceID]) / vec2(10.0);
    gl_Position = vec4(vec2(modifiedPosition) + offset, modifiedPosition.z, 1.0);
    instanceID = gl_InstanceID;
}