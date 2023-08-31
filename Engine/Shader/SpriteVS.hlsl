struct  VSInput {
    float32_t4 position : POSITION0;
    float32_t2 texcoord : TEXCOORD0;
    float32_t4 color : COLOR0;
};

struct VSOutput {
    float32_t4 position : SV_POSITION;
    float32_t2 texcoord : TEXCOORD0;
    float32_t4 color : COLOR0;
};

VSOutput main(VSInput input) {
    VSOutput output;

    output.position = input.position;
    output.texcoord = input.texcoord;
    output.color = input.color;

    return output;
}