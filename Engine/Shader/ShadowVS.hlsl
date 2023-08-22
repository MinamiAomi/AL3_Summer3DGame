#include "Common.hlsli"

struct  VSInput {
    float32_t3 position : POSITION0;
};

struct VSOutput {
    float32_t4 worldPosition : POSITION;
};

VSOutput main(VSInput input) {
    VSOutput output;

    output.worldPosition = mul(float32_t4(input.position, 1.0f), modelInstance_.worldMatrix);

    return output;
}