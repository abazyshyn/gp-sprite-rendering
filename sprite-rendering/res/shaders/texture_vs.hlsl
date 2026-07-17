/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};


//////////////
// TYPEDEFS //
//////////////
struct Vertex_s
{
    float4 position : POSITION0;
    float2 tex : TEXCOORD0;
};

struct PixelInput_s
{
    float4 position : SV_POSITION0;
    float2 tex : SV_TEXCOORD0;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader                                                              //
////////////////////////////////////////////////////////////////////////////////
PixelInput_s TextureVertexShader(Vertex_s input)
{
    PixelInput_s output;
    
    input.position.w = 1.0f;
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    output.tex = input.tex;
    
    return output;
}