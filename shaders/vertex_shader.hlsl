
cbuffer TransformationBuffer : register(b0)
{
	matrix ModelToWorldMatrix;
	matrix WorldToViewMatrix;
	matrix ProjectionMatrix;
};

struct VSIn
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float3 Binormal : BINORMAL;
	float2 TexCoord : TEX;
};

struct PSIn
{
	float4 Pos  : SV_Position;
	float3 Normal : NORMAL;
	float2 TexCoord : TEX;
	float3 WorldPos : POS;
};

//-----------------------------------------------------------------------------------------
// Vertex Shader
//-----------------------------------------------------------------------------------------

PSIn VS_main(VSIn input)
{
	PSIn output = (PSIn)0;
	
	// Model->View transformation
	matrix MV = mul(WorldToViewMatrix, ModelToWorldMatrix);

	// Model->View->Projection (clip space) transformation
	// SV_Position expects the output position to be in clip space
	matrix MVP = mul(ProjectionMatrix, MV);
	
	float TexScale = 1.0f;

	// Perform transformations and send to output
	output.Pos = mul(MVP, float4(input.Pos, 1));
	output.Normal = normalize(mul(ModelToWorldMatrix, float4(input.Normal,0)).xyz );
	output.TexCoord = float2(input.TexCoord.x*TexScale, input.TexCoord.y*TexScale);
	//output.TexCoord = input.TexCoord;
	output.WorldPos = mul(ModelToWorldMatrix, float4(input.Pos, 1)).xyz;
		
	return output;
}