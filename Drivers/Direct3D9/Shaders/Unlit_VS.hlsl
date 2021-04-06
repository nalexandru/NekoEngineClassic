/*
 * NekoEngine Unlit Material Shader
 */
 
struct VertexAttribs
{
	float4 position : POSITION;
	float2 UV : TEXCOORD0;
};

VertexAttribs
main(float3 aPosition : POSITION,
	float3 aNormal : NORMAL,
	float3 aTangent : TANGENT,
	float2 aUV : TEXCOORD0,
	uniform float4x4 uMVP)
{
	VertexAttribs ret;

	//ret.position = float4(aPosition, 1.0);
	ret.position = mul(uMVP, float4(aPosition, 1.0));
	ret.UV = aUV;

	return ret;
}