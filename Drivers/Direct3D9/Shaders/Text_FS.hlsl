/*
 * NekoEngine Text Fragment Shader
 */

struct VertexAttribs
{
	float4 position : POSITION;
	float4 color : COLOR;
	float2 UV : TEXCOORD0;
};

texture uTextMap;

sampler TextSampler = 
sampler_state
{
    Texture = <uTextMap>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

float4
main(VertexAttribs vAttr) : COLOR
{
	return float4(vAttr.color.xyz, vAttr.color.a * tex2D(TextSampler, vAttr.UV).r);
}
