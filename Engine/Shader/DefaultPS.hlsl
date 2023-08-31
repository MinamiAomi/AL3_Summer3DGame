#include "Common.hlsli"

struct VSOutput {
    float32_t4 position : SV_POSITION;
    float32_t3 normal : NORMAL0;
    float32_t2 texcoord : TEXCOORD0;

    float32_t4 worldPosition : TEXCOORD1;

#ifdef RECEIVE_SHADOW
    float32_t4 shadowCoord[NUM_SHADOW_MAPS] : TEXCOORD2;
#endif
};

struct PSOutput {
    float32_t4 color : SV_TARGET0;
};

float32_t4 TextureColor(VSOutput input) {
    float32_t4 color;
    color = texture_.Sample(textureSampler_, input.texcoord);
    color *= material_.color;
    return color;
}

float32_t4 ShadeColor(VSOutput input) {
    float32_t4 color;
    float32_t NdotL = dot(normalize(input.normal), -sceneConstant_.sunLightDirection);
    float32_t cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
    color = sceneConstant_.sunLightColor * cos * sceneConstant_.sunLightIntensity;
    color.w = 1.0f;
    return color;
}

#ifdef RECEIVE_SHADOW
float32_t Shadow(VSOutput input) {
   /* float32_t4 colorTable[3];
    colorTable[0] = float32_t4(1.0f, 0.0f, 0.0f, 1.0f);
    colorTable[1] = float32_t4(0.0f, 1.0f, 0.0f, 1.0f);
    colorTable[2] = float32_t4(0.0f, 0.0f, 1.0f, 1.0f);*/

    for (uint32_t i = 0; i < NUM_SHADOW_MAPS; i++) {
        float32_t depthInShadowSpace = input.shadowCoord[i].z / input.shadowCoord[i].w;
        if (depthInShadowSpace >= 0.0f && depthInShadowSpace <= 1.0f) {
            float32_t2 shadowMapUV = input.shadowCoord[i].xy / input.shadowCoord[i].w;
            shadowMapUV *= float32_t2(0.5f, -0.5f);
            shadowMapUV += 0.5f;

            if (shadowMapUV.x >= 0.0f && shadowMapUV.x <= 1.0f &&
                shadowMapUV.y >= 0.0f && shadowMapUV.y <= 1.0f) {
                float bias = 0.001f;
                return shadowMaps_.SampleCmpLevelZero(shadowSampler_, float32_t3(shadowMapUV, i), depthInShadowSpace - bias);
            }
        }
    }
    return 0.0f;
}
#endif

PSOutput main(VSOutput input) {

    PSOutput output;

    output.color = TextureColor(input);

    output.color *= ShadeColor(input);

#ifdef RECEIVE_SHADOW
    float32_t4 shadowColor = output.color * 0.5f;
    float32_t shadow = Shadow(input);
    output.color = lerp(output.color, shadowColor, shadow);
#endif
    

    output.color.w = 1.0f;
    return output;
}