
// this is an effect template. use it to start writing your own effects.

// -------------------------------------------------------------------------------------------------------------------------------------
// PARAMETERS:
// -------------------------------------------------------------------------------------------------------------------------------------

//transforms
float4x4 tW: WORLD;                  //the models world matrix
float4x4 tV: VIEW;                   //view matrix as set via Renderer (DX9)
float4x4 tP: PROJECTION;             //projection matrix as set via Renderer (DX9)
float4x4 tWVP: WORLDVIEWPROJECTION;  //WORLD*VIEW*PROJECTION


//the vvvv pins are defined here:
texture phase01TEX <string uiname="Phase01Texture";>;
texture phase02TEX <string uiname="Phase02Texture";>;
texture phase03TEX <string uiname="Phase03Texture";>;


float4x4 tTex <string uiname="Texture Transform";>; //Texture Transform
//

static const float TWO_PI = 6.28318530717958647693f;
float Treshold <string uiname="Treshold";>;
//float Var02 <string uiname="Var02";>;
//float4 Var03 <string uiname="Var03";>;



sampler2D phase01Map = sampler_state
{
     Texture = <phase01TEX>;
    AddressU = Wrap;
    AddressV = Wrap;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;

};

sampler2D phase02Map = sampler_state
{
     Texture = <phase02TEX>;
    AddressU = Wrap;
    AddressV = Wrap;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;

};

sampler2D phase03Map = sampler_state
{
     Texture = <phase03TEX>;
    AddressU = Wrap;
    AddressV = Wrap;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;

};

// -------------------------------------------------------------------------------------------------------------------------------------
// VERTEXSHADERS
// -------------------------------------------------------------------------------------------------------------------------------------

struct vs2ps
{
    float4 Pos  : POSITION;
    float2 TexC : TEXCOORD0;
};


vs2ps VSWARP(
    float4 Pos  : POSITION,
    float2 TexC : TEXCOORD)

{

    vs2ps OUT = (vs2ps)0;


    Pos = mul(Pos,tWVP);
   // TexC = mul(TexC, tTex);
    OUT.Pos  = Pos;
    OUT.TexC = TexC;


    return OUT;
}



//



// -------------------------------------------------------------------------------------------------------------------------------------
// PIXELSHADERS:
// -------------------------------------------------------------------------------------------------------------------------------------

float4 PSWARP(vs2ps IN): COLOR
{
    float3 color01 = tex2D( phase01Map, IN.TexC);
    float3 color02 = tex2D( phase02Map, IN.TexC);
    float3 color03 = tex2D( phase03Map, IN.TexC);

float4 warpColor = 0;

/*
warpColor[0] = color01[0];
warpColor[1] = color02[0];
warpColor[2] = color03[0];
*/

warpColor[0] = dot(color01.rgb, float3(0.3, 0.59, 0.11));
warpColor[1] = dot(color02.rgb, float3(0.3, 0.59, 0.11));
warpColor[2] = dot(color03.rgb, float3(0.3, 0.59, 0.11));

if(warpColor[0] <= Treshold || warpColor[1] <= Treshold || warpColor[1] <= Treshold )
{
warpColor = 0;
}

float4 OutColor =0;
OutColor = atan2(sqrt(3)*(warpColor[1]-warpColor[3]),2*warpColor[2]-warpColor[1]-warpColor[3])/TWO_PI;



OutColor[3] = 1;

return OutColor;
}



// -------------------------------------------------------------------------------------------------------------------------------------
// TECHNIQUES:
// -------------------------------------------------------------------------------------------------------------------------------------

technique WARP
{
    pass P0
    {
        VertexShader = compile vs_3_0 VSWARP();
        PixelShader  = compile ps_3_0 PSWARP();
    }
}
