#version 460

#extension GL_ARB_bindless_texture : require
precision highp float;
precision highp int;


layout(binding = 2) buffer MainTex {
    sampler2D textures[];
};

layout(binding = 3) buffer HeightTex {
    sampler2D heightMaps[];
};


in vec3 normalOut;
in vec3 FragPos;
in vec2 FragCoord;
flat in int instanceId;

uniform samplerCube shadowMap;
uniform vec3 lightPos;
uniform vec3 camPos;
uniform float farPlane;


out vec4 FragColor;

float falloffDistance = 200.0;
float specularStrength = 1.0;

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
    vec3( 0.423, -0.032, -0.904), vec3(-0.419,  0.041, -0.893)
);

float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

mat3 getPoissonRotation(vec3 seed) {
    float angle = rand(seed.xy * 40.0) * 6.2831853;
    float s = sin(angle);
    float c = cos(angle);
    return mat3(
        c, 0, s,
        0, 1, 0,
       -s, 0, c
    );
}

// Receiver-plane bias to eliminate acne, based on world-space normal and light dir
float receiverPlaneBias(vec3 normal, vec3 lightDir, float depth) {
    float ndotl = max(dot(normal, lightDir), 0.001);
    return 0.002 * (1.0 / ndotl); // Conservative bias scaling
}

    const int NUM_SAMPLES = 20;
    const float SEARCH_RADIUS = 0.05;
    const float MAX_DISTANCE_BLUR = 0.25;
    const float MIN_FILTER_RADIUS = 0.005;
    const float MAX_FILTER_RADIUS = 0.4;
    const float BASE_PENUMBRA_SCALE = 0.5;

float PCSS_Shadow(vec3 fragPos)
{

    vec3 toLight = fragPos - lightPos;
    float currentDepth = length(toLight);
    vec3 lightDir = toLight / currentDepth;
    vec3 normal = normalize(normalOut);
    float shadowLength = currentDepth - texture(shadowMap, toLight).r;

    // Bias to prevent acne
    float bias = receiverPlaneBias(normal, lightDir, currentDepth);

    // Rotate Poisson disk
    mat3 rot = getPoissonRotation(fragPos);

    // --- Blocker Search Phase ---
    float blockerDepthSum = 0.0;
    int blockerCount = 0;

    float sampleOffsetBase = SEARCH_RADIUS * shadowLength;

    for (int i = 0; i < NUM_SAMPLES; ++i)
    {
        vec3 offset = rot * poissonDisk[i] * sampleOffsetBase;
        float sampleDepth = texture(shadowMap, toLight + offset).r * farPlane;
        if (sampleDepth < currentDepth - bias)
        {
            blockerDepthSum += sampleDepth;
            ++blockerCount;
        }
    }

    float avgBlockerDepth = (blockerCount == 0)
        ? currentDepth - bias * 2.0
        : blockerDepthSum / float(blockerCount);

    // --- Penumbra Estimation ---
    float penumbraRatio = (currentDepth - avgBlockerDepth) / avgBlockerDepth;

    // --- Distance-Based Blur Scale ---
    float distanceFactor = clamp(currentDepth / falloffDistance, 0.0, 1.0);
    distanceFactor = pow(distanceFactor, 1.5);

    float filterRadius = penumbraRatio * BASE_PENUMBRA_SCALE
                       + mix(0.0, MAX_DISTANCE_BLUR, distanceFactor);
    filterRadius = clamp(filterRadius, MIN_FILTER_RADIUS, MAX_FILTER_RADIUS);

    // --- PCF Filtering ---
    float shadow = 0.0;
    for (int i = 0; i < NUM_SAMPLES; ++i)
    {
        vec3 offset = rot * poissonDisk[i] * filterRadius;
        float sampleDepth = texture(shadowMap, toLight + offset).r * farPlane;
        shadow += step(sampleDepth, currentDepth - bias);
    }

    shadow *= 1.0 / float(NUM_SAMPLES);

    return 1.0 - shadow;
}

vec3 aces(vec3 x) {
  const float a = 2.51;
  const float b = 0.03;
  const float c = 2.43;
  const float d = 0.59;
  const float e = 0.14;
  return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

vec4 GetTextureColor()
{

    sampler2D textureSampler = textures[instanceId];
    vec4 objectColor = texture(textureSampler, FragCoord * 4);
    objectColor = pow(objectColor, vec4(2.0));
    objectColor = vec4(aces(objectColor.rgb), 1.0);
    return objectColor;
}

vec4 NormalLightImproved()
{

    float ambientStrength = 0.2;
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec4 objectColor = GetTextureColor();

    vec3 norm = normalize(normalOut);
    vec3 lightDir = normalize(lightPos - FragPos);
    float distance = length(lightPos - FragPos);

    
    float attenuation = clamp(1.0 - distance / falloffDistance, 0.0, 1.0);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 ambient = ambientStrength * lightColor;

    vec3 viewDir = normalize(camPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec3 specular = specularStrength * spec * lightColor;

    float shadow = PCSS_Shadow(FragPos);

    vec3 lighting = ambient + shadow * (diffuse + specular);

    // Apply attenuation to the lighting
    vec3 result = attenuation * lighting * vec3(objectColor);

    return vec4(result, 1.0);
}

void main()
{

    FragColor = NormalLightImproved();

}