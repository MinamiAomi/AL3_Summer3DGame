struct ModelInstance {
    float32_t4x4 worldMatrix;
};
ConstantBuffer<ModelInstance> modelInstance_ : register(b0);

struct SceneConstant {
    float32_t4x4 cameraVPMatrix;
    float32_t3 cameraPosition;
    float32_t sunLightIntensity;
    float32_t4 sunLightColor;
    float32_t3 sunLightDirection;
};
ConstantBuffer<SceneConstant> sceneConstant_ : register(b1);

struct Material {
    float32_t4 color;
};
ConstantBuffer<Material> material_ : register(b2);

Texture2D<float32_t4> texture_ : register(t0);
Texture2DArray<float32_t> shadowMaps_ : register(t1);

StructuredBuffer<float32_t4x4> shadowMatrices_ : register(t2);

SamplerState textureSampler_ : register(s0);
SamplerComparisonState shadowSampler_ : register(s1);

#define NUM_SHADOW_MAPS 3