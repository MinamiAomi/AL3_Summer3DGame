Texture2D<float32_t4> texture_ : register(t0);

SamplerState textureSampler_ : register(s0);

struct VSOutput {
    float32_t4 position : SV_POSITION;
    float32_t2 texcoord : TEXCOORD0;
    float32_t4 color : COLOR0;
};

struct PSOutput {
    float32_t4 color : SV_TARGET0;
};

float32_t4 TextureColor(VSOutput input) {
    float32_t4 color;
    color = texture_.Sample(textureSampler_, input.texcoord);
    return color;
}

PSOutput main(VSOutput input) {

    PSOutput output;

    output.color = TextureColor(input);
    output.color *= input.color;
    //output.color = float32_t4(1.0f, 1.0f,1.0f,1.0f);

    return output;
}