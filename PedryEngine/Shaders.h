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

vec3 rgb2hsv(vec3 a) 
{
    vec3 c = vec3(a.x, a.z, a.y);

    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c) 
{
    vec3 p = abs(fract(c.xxx + vec3(0.0, 1.0/3.0, 2.0/3.0)) * 6.0 - 3.0);
    return c.z * mix(vec3(1.0), clamp(p - 1.0, 0.0, 1.0), c.y);
}

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
    vec3 lVec = fragPos - lightPos;
    float curDepth = length(lVec);
    vec3 lDir = lVec / curDepth;
    vec3 norm = normalize(normalOut);

    float bias = max(0.1 * (1.0 - dot(norm, lDir)), 0.005);

    float rnd = rand(fragPos.xy * 10.0) * 6.2831;
    float s = sin(rnd);
    float c = cos(rnd);

    mat3 rot = mat3(
        c, 0, s,
        0, 1, 0,
       -s, 0, c
    );

    float avg = 0.0;
    int cnt = 0;
    float radius = 0.05;

    vec3 off, samp;
    float sampDepth;

    for (int i = 0; i < 20; ++i) 
    {
        off = rot * poissonDisk[i] * radius;
        samp = lVec + off;
        sampDepth = texture(shadowMap, samp).r * farPlane;

        if (sampDepth < curDepth - bias) 
        {
            avg += sampDepth;
            cnt++;
        }
    }

    if (cnt == 0) return 1.0;

    avg /= float(cnt);
    float penumbra = (curDepth - avg) / avg;
    float filterR = clamp(penumbra * 0.5, 0.01, 0.3);

    float shadow = 0.0;
    for (int i = 0; i < 20; ++i)
    {
        off = rot * poissonDisk[i] * filterR;
        samp = lVec + off;
        sampDepth = texture(shadowMap, samp).r * farPlane;

        if (curDepth - bias > sampDepth) shadow += 1.0;
    }

    return 1.0 - (shadow * 0.05); // shadow /= 20.0
}



vec4 NormalLightImproved()
{
    sampler2D t = textures[0];
    vec4 tex = texture(t, FragCoord);

    vec3 norm = normalize(normalOut);
    vec3 lVec = lightPos - FragPos;
    float dist = length(lVec);
    vec3 lDir = lVec / dist;

    float atten = 1.0 - dist / 100.0;
    atten = clamp(atten, 0.0, 1.0);

    float diff = max(dot(norm, lDir), 0.0);
    vec3 light = vec3(1.0); // white light

    vec3 diffLight = diff * light;

    vec3 ambient = 0.1 * light;

    vec3 vDir = normalize(camPos - FragPos);
    vec3 rDir = reflect(-lDir, norm);

    float spec = pow(max(dot(vDir, rDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * light;

    float shad = PCSSS(FragPos);

    diffLight = (ambient + shad * (diffLight + specular)) * atten;

    // Reuse tex.rgb as base color, convert to HSV
    tex.rgb = rgb2hsv(tex.rgb);
    tex.r *= 1.0; // H stays
    tex.g *= 1.0; // S stays

    tex.b *= clamp(dot(diffLight, vec3(0.333)), 0.0, 10.0); // apply to V
    tex.rgb = hsv2rgb(tex.rgb);

    return tex;
}

void main()
{

    vec3 hsv = rgb2hsv(vec3(0.0, 0.0, 0.5));
    vec3 rgb = hsv2rgb(hsv);

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