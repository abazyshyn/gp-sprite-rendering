/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture : register(t0);
SamplerState sampleType : register(s0);

cbuffer TranslationBuffer
{
    float textureOffset;
};

//////////////
// TYPEDEFS //
//////////////
struct PixelInput_s
{
    float4 position : SV_POSITION0;
    float2 tex : TEXCOORD0;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader                                                               //
////////////////////////////////////////////////////////////////////////////////
float4 TexturePixelShader(PixelInput_s input) : SV_TARGET
{
    input.tex.x += textureOffset;
    float4 textureColor = shaderTexture.Sample(sampleType, input.tex);

    return textureColor;
}