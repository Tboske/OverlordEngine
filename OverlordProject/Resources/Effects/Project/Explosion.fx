float4x4 gWorldViewProj : WORLD;
float4x4 gWorld;

Texture2D gTexture;

//STATES
//******
SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = WRAP;
    AddressV = WRAP;
};

BlendState AlphaBlending
{
    BlendEnable[0] = TRUE;
    SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
    BlendOp = ADD;
    SrcBlendAlpha = ONE;
    DestBlendAlpha = ZERO;
    BlendOpAlpha = ADD;
    RenderTargetWriteMask[0] = 0x0f;
};

DepthStencilState DisableDepthWriting
{
	//Enable Depth Rendering
    DepthEnable = TRUE;
	//Disable Depth Writing
    DepthWriteMask = 0;
};


RasterizerState BackCulling
{
    CullMode = BACK;
};

struct VS_DATA
{
    float3 Position : POSITION;
    float Size : TEXCOORD0;
};

struct GS_DATA
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
};

// vertex shader
VS_DATA MainVS(VS_DATA data)
{
    return data;
}

// geomety shader
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float2 texCoord)
{
    GS_DATA output = (GS_DATA) 0;
    
    output.Position = mul(float4(pos, 1.f), gWorldViewProj);
    
    output.TexCoord = texCoord;
    
    //texCoord -= float2(0.5f, 0.5f);
    //texCoord = mul(texCoord, uvRotation);
    //texCoord += float2(0.5f, 0.5f);
    
    
    triStream.Append(output);
}

[maxvertexcount(4)]
void MainGS(point VS_DATA vertex[1], inout TriangleStream<GS_DATA> triStream)
{
    //Use these variable names
    float3 topLeft, topRight, bottomLeft, bottomRight;
    const float3 origin = vertex[0].Position;
    
    topLeft     = float3(origin.x   - vertex[0].Size , origin.y , origin.z   - vertex[0].Size );
    topRight    = float3(origin.x   + vertex[0].Size , origin.y , origin.z   - vertex[0].Size );
    bottomLeft  = float3(origin.x   - vertex[0].Size , origin.y , origin.z   + vertex[0].Size );
    bottomRight = float3(origin.x   + vertex[0].Size , origin.y , origin.z   + vertex[0].Size );    

    
    
	CreateVertex(triStream, bottomLeft	, float2(0,1));
	CreateVertex(triStream, bottomRight	, float2(1,1));
	CreateVertex(triStream, topLeft		, float2(0,0));
	CreateVertex(triStream, topRight	, float2(1,0));
}

//PIXEL SHADER
//************
float4 MainPS(GS_DATA input) : SV_TARGET
{
	//Simple Texture Sampling
    return gTexture.Sample(samPoint, input.TexCoord);
    //return float4(1, 1, 1, 1);
}

// Default Technique
technique10 Default
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, MainVS()));
        SetGeometryShader(CompileShader(gs_4_0, MainGS()));
        SetPixelShader(CompileShader(ps_4_0, MainPS()));
		
        SetRasterizerState(BackCulling);
        SetDepthStencilState(DisableDepthWriting, 0);
        SetBlendState(AlphaBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
    }
}
