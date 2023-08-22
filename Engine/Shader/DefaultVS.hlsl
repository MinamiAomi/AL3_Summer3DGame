#include "Common.hlsli"

struct  VSInput {
    float32_t3 position : POSITION0;
    float32_t3 normal : NORMAL0;
    float32_t2 texcoord : TEXCOORD0;
};

struct VSOutput {
    float32_t4 position : SV_POSITION;
    float32_t3 normal : NORMAL0;
    float32_t2 texcoord : TEXCOORD0;

    float32_t4 worldPosition : TEXCOORD1;

#ifdef RECEIVE_SHADOW
    float32_t4 shadowCoord[NUM_SHADOW_MAPS] : TEXCOORD2;
#endif
};

VSOutput main(VSInput input) {
    VSOutput output;

    output.worldPosition = mul(float32_t4(input.position, 1.0f), modelInstance_.worldMatrix);

    output.position = mul(output.worldPosition, sceneConstant_.cameraVPMatrix);
    output.normal = mul(input.normal, (float32_t3x3)modelInstance_.worldMatrix);
    output.texcoord = input.texcoord;

#ifdef RECEIVE_SHADOW
    for (uint32_t i = 0; i < NUM_SHADOW_MAPS; i++) {
        output.shadowCoord[i] = mul(output.worldPosition, shadowMatrices_[i]);
    }
#endif

    return output;
}