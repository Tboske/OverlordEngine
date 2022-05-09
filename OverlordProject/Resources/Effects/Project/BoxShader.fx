float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION; 
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);
float3 gColorAdd = float3(0.f, 0.f, 0.f);
bool gUseColor = false;

Texture2D gDiffuseMap;
Texture2D gNormalMap;
bool gFlipGreenChannel;


SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;// or Mirror or Clamp or Border
    AddressV = Wrap;// or Mirror or Clamp or Border
};

RasterizerState Solid
{
	FillMode = SOLID;
	CullMode = FRONT;
};

struct VS_INPUT{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float3 tangent: TANGENT;
	float2 texCoord : TEXCOORD;
};
struct VS_OUTPUT{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float3 tangent: TANGENT;
	float2 texCoord : TEXCOORD;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

RasterizerState NoCulling
{
	CullMode = NONE;
};

BlendState NoBlending
{
	BlendEnable[0] = FALSE;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input){

	VS_OUTPUT output;
	// Step 1:	convert position into float4 and multiply with matWorldViewProj
	output.pos = mul ( float4(input.pos,1.0f), gWorldViewProj );
	// Step 2:	rotate the normal: NO TRANSLATION
	//			this is achieved by clipping the 4x4 to a 3x3 matrix, 
	//			thus removing the postion row of the matrix
	output.normal = normalize(mul(input.normal, (float3x3)gWorld));
	output.tangent = mul(input.tangent, (float3x3)gWorld);
	output.texCoord = input.texCoord;
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET{

	float4 diffuseColor = gDiffuseMap.Sample( samLinear,input.texCoord );

	float3 color_rgb = diffuseColor.rgb;
	if (gUseColor)
		color_rgb = gColorAdd;
	float color_a = diffuseColor.a;


	
	// normal
	float3 binormal = normalize(cross(input.tangent, input.normal));
	if (gFlipGreenChannel)
		binormal = -binormal;
	float3x3 localAxis = float3x3(input.tangent, binormal, input.normal);

	float3 sampledNormal = gNormalMap.Sample(samLinear, input.texCoord);
	sampledNormal = sampledNormal * 2.f - float3(1.f, 1.f, 1.f);	// move from 0_1 range into -1_1 range

	float3 newNormal = normalize(mul(sampledNormal, localAxis));

	//HalfLambert Diffuse :)
	float diffuseStrength = dot(newNormal, -gLightDirection);
	diffuseStrength = diffuseStrength * 0.5 + 0.5;
	diffuseStrength = saturate(diffuseStrength);
	color_rgb = color_rgb * diffuseStrength;

	return float4( color_rgb, color_a );
}

//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
technique11 Default
{
    pass P0
    {
		SetRasterizerState(NoCulling);
		SetDepthStencilState(EnableDepth, 0);
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);

		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

