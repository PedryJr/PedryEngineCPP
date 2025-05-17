#pragma once

#define Vert3D R"V0G0N(
#version 460
precision highp float;
precision highp int;
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUv;

layout(std430, binding = 1) buffer Matrices {
    mat4 modelMatrices[]; // Storage for matrices
};

uniform mat4 rotationMatrix;
uniform mat4 cameraMatrix;

out vec3 normalOut;
out vec3 FragPos;
out vec2 FragCoord;

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
#version 460

#extension GL_ARB_bindless_texture : require
precision highp float;
precision highp int;


layout(binding = 2, std430) readonly buffer ssbo3 {
    sampler2D textures[];
};


in vec3 normalOut;
in vec3 FragPos;
in vec2 FragCoord;

uniform samplerCube shadowMap;
uniform vec3 lightPos;
uniform vec3 camPos;
uniform float farPlane;


out vec4 FragColor;

float specularStrength = 0.5;

const vec3 poissonDisk[20] = vec3[](
    vec3( 0.186,  0.512,  0.623), vec3(-0.320,  0.499,  0.521),
    vec3( 0.328, -0.418,  0.703), vec3(-0.285, -0.357,  0.543),
    vec3( 0.719,  0.132, -0.321), vec3(-0.718, -0.189,  0.336),
    vec3( 0.231,  0.247, -0.773), vec3(-0.230, -0.258, -0.702),
    vec3( 0.561, -0.676,  0.218), vec3(-0.601,  0.679, -0.224),
    vec3( 0.132,  0.890, -0.221), vec3(-0.145, -0.811, -0.321),
    vec3( 0.387,  0.135,  0.912), vec3(-0.425, -0.209,  0.895),
    vec3( 0.898, -0.301,  0.099), vec3(-0.905,  0.287,  0.065),
    vec3( 0.127,  0.467, -0.877), vec3(-0.132, -0.477,  0.869),
    vec3( 0.423, -0.032, -0.904), vec3(-0.419,  0.041, -0.893));

float rand(vec2 co) 
{ return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453); }


#define BLOCKER_SEARCH_SAMPLES 16
#define PCF_SAMPLES 32

float PCSSS(vec3 fragPos) 
{

    vec3 normal = normalize(normalOut);

    vec3 fragToLight = fragPos - lightPos;
    float currentDepth = length(fragToLight);
    vec3 lightDir = normalize(fragToLight);
    
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    float randomAngle = rand(fragPos.xy * 10.0) * 6.2831;
    mat3 rotationMatrix = mat3(
        cos(randomAngle), 0, sin(randomAngle),
        0, 1, 0,
       -sin(randomAngle), 0, cos(randomAngle)
    );

    float avgBlockerDepth = 0.0;
    int blockerCount = 0;
    float searchRadius = 0.05;

    for (int i = 0; i < 20; ++i) 
    {
        vec3 offset = rotationMatrix * poissonDisk[i] * searchRadius;
        float sampleDepth = texture(shadowMap, fragToLight + offset).r * farPlane;
        if (sampleDepth < currentDepth - bias) 
        {
            avgBlockerDepth += sampleDepth;
            blockerCount++;
        }
    }

    if (blockerCount == 0) return 1.0;

    avgBlockerDepth /= float(blockerCount);

    float penumbraRatio = (currentDepth - avgBlockerDepth) / avgBlockerDepth;
    float lightSize = 0.5;
    float filterRadius = clamp(penumbraRatio * lightSize, 0.01, 0.3);

    float shadow = 0.0;
    for (int i = 0; i < 20; ++i) 
    {
        vec3 offset = rotationMatrix * poissonDisk[i] * filterRadius;
        float sampleDepth = texture(shadowMap, fragToLight + offset).r * farPlane;
        if (currentDepth - bias > sampleDepth) shadow += 1.0;
    }

    shadow /= 20.0;
    return 1.0 - shadow;
}


vec4 NormalLightImproved()
{
    sampler2D textureSampler = textures[0];
    float ambientStrength = 0.1;
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec4 objectColor = texture(textureSampler, FragCoord);

    vec3 norm = normalize(normalOut);
    vec3 lightDir = normalize(lightPos - FragPos);
    float distance = length(lightPos - FragPos);

    float falloffDistance = 50.0;
    
    float attenuation = clamp(1.0 - distance / falloffDistance, 0.0, 1.0);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 ambient = ambientStrength * lightColor;

    vec3 viewDir = normalize(camPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    float shadow = PCSSS(FragPos);

    vec3 lighting = ambient + shadow * (diffuse + specular);

    // Apply attenuation to the lighting
    vec3 result = attenuation * lighting * vec3(objectColor);

    return vec4(result, 1.0);
}

void main()
{

    FragColor = NormalLightImproved();

}
)V0G0N"

#define VertShadow R"V0G0N(
#version 460


layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUv;

layout(std430, binding = 1) buffer Matrices {
    mat4 modelMatrices[];
};


void main() {
    gl_Position = modelMatrices[gl_InstanceID] * vec4(vertexPosition, 1.0);
}

)V0G0N"

#define GeomShadow R"V0G0N(
#version 460

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;


uniform mat4 shadowMatrices[6];

out vec4 FragPos;

void main() {
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face;
        for(int i = 0; i < 3; ++i)
        {
            FragPos = gl_in[i].gl_Position;
            gl_Position = shadowMatrices[face] * FragPos;
            EmitVertex();
        }    
        EndPrimitive();
    }
}

)V0G0N"

#define FragShadow R"V0G0N(
#version 460

in vec4 FragPos;

uniform vec3 lightPos;
uniform float farPlane;

void main() {

    float lightDistance = length(FragPos.xyz - lightPos);
    lightDistance = lightDistance / farPlane;
    gl_FragDepth = lightDistance;
}

)V0G0N"

//vec3 modelMatrix = modelMatrices[gl_InstanceID];
//
//vec3 modifiedPosition = position / modelMatrix.z);
//vec2 offset = vec2(modelMatrix) / vec2(10.0);
//gl_Position = vec4(vec2(modifiedPosition) + offset, 0, 1.0);
//color = vec4(0.25, 0.25, 0.25 / 0.25, 0.1);