
Texture2D texDiffuse : register(t0);
Texture2D texNormal : register(t1);
SamplerState texSampler : register(s0);

struct PSIn
{
	float4 Pos  : SV_Position;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float3 Binormal : BINORMAL;
	float2 TexCoord : TEX;
	float3 WorldPos : POS;
};

cbuffer CameraAndLightBuffer : register(b0)
{
	float3 CamPosition;
	float3 LightPosition1;
	float3 LightPosition2;
};

cbuffer MaterialAndShininessBuffer : register(b1)
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
}

//-----------------------------------------------------------------------------------------
// Pixel Shader
//-----------------------------------------------------------------------------------------
//

float4 PS_main(PSIn input) : SV_Target
{
	float4 DiffuseTexColor = texDiffuse.Sample(texSampler, input.TexCoord);
	float4 NormalTexColor = texNormal.Sample(texSampler, input.TexCoord);

	float3 T = normalize(input.Tangent);
	float3 B = normalize(input.Binormal);
	float3 N = normalize(input.Normal);
	float3x3 TBN = transpose(float3x3(T,B,N));
	float3 WorldNormal = mul(TBN, NormalTexColor.xyz);

	float3 LightVector1 = normalize(LightPosition1 - input.WorldPos);
	//float3 ReflectVector1 = normalize(reflect(-LightVector1, input.Normal));
	float3 ReflectVector1 = normalize(reflect(LightVector1, WorldNormal));

	float3 LightVector2 = normalize(LightPosition2 - input.WorldPos);
	//float3 ReflectVector2 = normalize(reflect(LightVector2, input.Normal));
	float3 ReflectVector2 = normalize(reflect(LightVector2, WorldNormal));

	float3 ViewVector = normalize(CamPosition - input.WorldPos);

	float Shininess = Specular.w;

	//float3 LightSum = 
	//	(mul(DiffuseTexColor.xyz, dot(LightVector1, input.Normal) + mul(Specular.xyz, pow(max(dot(ReflectVector1, ViewVector), 0), Shininess))) +
	//	(mul(DiffuseTexColor.xyz, dot(LightVector2, input.Normal)) + mul(Specular.xyz, pow(max(dot(ReflectVector2, ViewVector), 0), Shininess))));

	float3 A = Ambient.xyz;
	float3 D = mul(DiffuseTexColor.xyz, dot(LightVector1, input.Normal));
	//float3 D = mul(DiffuseTexColor.xyz, dot(LightVector1, WorldNormal)); // Normal Map included
	float3 S = mul(Specular.xyz, pow(max(dot(ReflectVector1, ViewVector), 0), Shininess));

	//return float4(S,1);
	return float4(A * 0.25f + D + S, 1); //one light source + diffuse texture
}
