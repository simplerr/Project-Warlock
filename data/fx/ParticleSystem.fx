cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
	uniform extern float3   gEyePosW;
	uniform extern float3   gAccel;
	uniform extern float    gTime;
	uniform extern int      gViewportHeight;
}

Texture2D gTexture;

SamplerState TexS
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU  = CLAMP;
    AddressV  = CLAMP;
};
 
struct OutputVS
{
    float4 posH  : POSITION0;
	float4 color : COLOR0;
    float2 tex0  : TEXCOORD0; // D3D fills in for point sprites.
    float size   : PSIZE; // In pixels.
};

//! Geometry shader that expands a point to a quad.
[maxvertexcount(4)]
void GS(point VertexOut gin[1], inout TriangleStream<GeoOut> triStream)
{	
	// Compute the local coordinate system of the sprite relative to the world
	// space such that the billboard is aligned with the y-axis and faces the eye.
	float3 up = float3(0.0f, 1.0f, 0.0f);
	float3 look = gEyePosW - gin[0].CenterW;
	look.y = 0.0f; // y-axis aligned, so project to xz-plane	[NOTE]
	look = normalize(look);
	float3 right = cross(up, look);

	// Compute triangle strip vertices (quad) in world space.
	float halfWidth  = 0.5f*gin[0].SizeW.x;
	float halfHeight = 0.5f*gin[0].SizeW.y;
	
	float4 v[4];
	v[0] = float4(gin[0].CenterW + halfWidth*right - halfHeight*up, 1.0f);
	v[1] = float4(gin[0].CenterW + halfWidth*right + halfHeight*up, 1.0f);
	v[2] = float4(gin[0].CenterW - halfWidth*right - halfHeight*up, 1.0f);
	v[3] = float4(gin[0].CenterW - halfWidth*right + halfHeight*up, 1.0f);

	// Transform quad vertices to world space and output them as a triangle strip.
	GeoOut gout;
	[unroll]
	for(int i = 0; i < 4; ++i)
	{
		gout.PosH     = mul(v[i], gViewProj);
		gout.PosW     = v[i].xyz;
		gout.NormalW  = look;
		gout.Tex      = gTexC[i];
		
		triStream.Append(gout);
	}
}

OutputVS ParticleVS(float3 posL    : POSITION0, 
                     float3 vel     : TEXCOORD0,
                     float size     : TEXCOORD1,
                     float time     : TEXCOORD2,
                     float lifeTime : TEXCOORD3,
                     float mass     : TEXCOORD4,
                     float4 color   : COLOR0)
{
    // Zero out our output.
	OutputVS outVS = (OutputVS)0;

	// Get age of particle from creation time.
	float t = gTime - time;

	// Constant acceleration.
	posL = posL + vel*t + 0.5f * gAccel * t * t;

	// Transform to homogeneous clip space.
	outVS.posH = mul(float4(posL, 1.0f), gWorldViewProj);

	// Compute size as a function of the distance from the camera,
	// and the viewport heights.  The constants were found by 
	// experimenting.
	float d = distance(posL, gEyePosW);
	outVS.size = gViewportHeight*size/(1.0f + 8.0f*d);

	outVS.color = color;

	// Done--return the output.
    return outVS;
}

float4 ParticlePS(float4 color : COLOR0, float2 tex0 : TEXCOORD0) : COLOR
{
	return tex2D(TexS, tex0) * color;
}

technique ParticleTech
{
    pass P0
    {
		SetVertexShader(CompileShader( vs_5_0, ParticleVS()));
		SetGeometryShader(NULL);
        SetPixelShader(CompileShader( ps_5_0, ParticlePS()));
        
        //PointSpriteEnable = true;
        AlphaBlendEnable = true;

	    // Don't write to depth buffer; that is, particles don't obscure
	    // the objects behind them.
	    ZWriteEnable = false;
    }
}