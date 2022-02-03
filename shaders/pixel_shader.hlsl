
Texture2D texDiffuse : register(t0);
Texture2D texNormal : register(t1);
SamplerState texSampler : register(s0);

struct PSIn
{
	float4 Pos  : SV_Position;
	float3 Normal : NORMAL;
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
	//return float4(input.TexCoord, 0, 1);
	return texDiffuse.Sample(texSampler, input.TexCoord);
	/*float3 LightVector1 = normalize(LightPosition1 - input.WorldPos);
	float3 ReflectVector1 = normalize(reflect(-LightVector1, input.Normal));*/

	//float3 LightVector2 = normalize(LightPosition2 - input.WorldPos);
	//float3 ReflectVector2 = normalize(reflect(LightVector2, input.Normal));

	//float3 ViewVector = normalize(CamPosition - input.WorldPos);

	//float4 DiffuseTexColor = texDiffuse.Sample(texSampler, input.TexCoord);
	//float4 NormalTexColor = texNormal.Sample(texSampler, input.TexCoord);

	//float3 LightSum = 
	//	(mul(Diffuse.xyz, LightVector1 * input.Normal) + mul(Specular.xyz, pow(ReflectVector1 * ViewVector, 1))) +
	//	(mul(Diffuse.xyz, LightVector2 * input.Normal) + mul(Specular.xyz, pow(ReflectVector2 * ViewVector, 1)));

	//float Shininess = Specular.w;
	
	//return float4(Ambient.xyz + LightSum, 1.0f); //multiple light sources

	//float3 A = Ambient.xyz;
	//float3 D = mul(Diffuse.xyz, dot(LightVector1, input.Normal));
	//float3 S = mul(Specular.xyz, pow(max(dot(ReflectVector1, ViewVector), 0), Shininess));

	//float3 DColor = mul(DiffuseTexColor.xyz, dot(LightVector1, input.Normal));

	//return float4(DColor,1);
	//return float4(A + D + S, 1);
	//return float4(Ambient.xyz + mul(Diffuse.xyz, LightVector1 * input.Normal) + mul(Specular.xyz, pow(max(ReflectVector1 * ViewVector, 0), 200.0f)), 1); //one light source, FRÅGA: whats up with shininess?
	//return float4(A + DColor + S, 1); //one light source + diffuse texture
}
