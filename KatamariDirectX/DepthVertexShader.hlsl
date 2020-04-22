cbuffer MatrixBuffer
{
    matrix WVP;
};

struct VS_INPUT
{
    float4 position : POSITION;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 depthPosition : TEXTURE0;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
	
    input.position.w = 1.0f;
    output.position = mul(input.position, WVP);
	// Пишем позицию в depthPosition
    output.depthPosition = output.position;
	
    return output;
}