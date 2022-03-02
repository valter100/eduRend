
Texture2D texDiffuse : register(t0);
Texture2D texNormal : register(t1);
TextureCube texCube : register (t2);
SamplerState texSampler : register(s0);
SamplerState cubeSampler : register(s1);

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
	float4 LightPositions[2];
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

float4 PS_main(PSIn input) : SV_Target
{
	//Phong effect components: Ambient, Diffuse & Specular
	//--------------------------------------------------------------------------------------------------------

	float3 A; //Ambience
	float3 D; //Diffuse
	float3 S; //Specular

	//Get Texture values from Input Textures: Diffuse & Normal
	//--------------------------------------------------------------------------------------------------------

	float4 DiffuseTexColor = texDiffuse.Sample(texSampler, input.TexCoord); //Color values
	float4 NormalTexVector = texNormal.Sample(texSampler, input.TexCoord) * 2 - 1; //Vector values

	//Calculating TBN-matrix from each vertex's Tangent, Binormal & Normal
	//--------------------------------------------------------------------------------------------------------

	float3 T = normalize(input.Tangent);
	float3 B = normalize(input.Binormal);
	float3 N = normalize(input.Normal);
	float3x3 TBN = transpose(float3x3(T,B,N));
	float3 WorldNormal = mul(TBN, NormalTexVector.xyz);

	//Cube Mapping & Reflection Vector Calculations:
	//--------------------------------------------------------------------------------------------------------
	
	float3 ViewVector = normalize(input.WorldPos - CamPosition);
	float3 ReflectVector = normalize(reflect(ViewVector, WorldNormal));
	float4 CubeTexVector = texCube.Sample(cubeSampler, ReflectVector);

	//light Vector Calculations: Light & Reflection with Diffuse & Specular components
	//--------------------------------------------------------------------------------------------------------

	float3 LightSum;
	float count = LightPositions[0].w;

	float Shininess = Specular.w;

	for (int i = 0; i < count; i++)
	{
		float3 LightDistance = LightPositions[i].xyz - input.WorldPos; //attenuation
		float3 LightVector = normalize(LightDistance);
		float3 LightReflectVector = normalize(reflect(LightVector, WorldNormal)); //With normal mapping
		
		//float3 SpecularReflection = mul(Specular.xyz, CubeTexVector.xyz); // * or mul()?
		float3 SpecularReflection = Specular.xyz * CubeTexVector.xyz; // * or mul()?
		
		D = mul(DiffuseTexColor.xyz, max(dot(LightVector, WorldNormal),0)); //With normal mapping (should input.Normal be WorldNormal?), Wack because not all models have normal tex?
        S = mul(Specular.xyz, pow(max(dot(LightReflectVector, ViewVector), 0), Shininess));

		LightSum += D + S;
	}
	
	//Ambience
	//--------------------------------------------------------------------------------------------------------

	A = Ambient.xyz;

	//Final Calculation taking all components into account
	//--------------------------------------------------------------------------------------------------------

	return float4(A * 0.25f + LightSum, 1); //support multiple light sources + diffuse texture and normal maps
}
