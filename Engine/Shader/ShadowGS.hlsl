#include "Common.hlsli"

struct VSOutput {
    float32_t4 worldPosition : POSITION;
};

struct GSOutput {
    float32_t4 position : SV_POSITION;
    uint32_t rtIndex : SV_RenderTargetArrayIndex;
};

#define NUM_VERTICES 3

[maxvertexcount(NUM_VERTICES * NUM_SHADOW_MAPS)]
void main(
    triangle VSOutput input[NUM_VERTICES],
    inout TriangleStream<GSOutput> output) {

    GSOutput element;
    for (uint32_t i = 0; i < NUM_SHADOW_MAPS; i++) {
        element.rtIndex = i;
        for (uint32_t j = 0; j < NUM_VERTICES; j++) {
            element.position = mul(input[j].worldPosition, shadowMatrices_[i]);
            output.Append(element);
        }
        output.RestartStrip();
    }
}