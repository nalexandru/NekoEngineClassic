/*
 * NekoEngine Unlit Material Shader
 */
 
struct VertexAttribs
{
	float4 position : POSITION;
	float2 UV : TEXCOORD0;
};

float4 uDiffuseColor;

texture uDiffuseMap;

sampler TextureSampler = 
sampler_state
{
    Texture = <uDiffuseMap>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

float4
main(VertexAttribs vAttr) : COLOR
{
	return uDiffuseColor * tex2D(TextureSampler, vAttr.UV);
}
