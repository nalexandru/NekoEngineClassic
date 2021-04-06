/*
 * NekoEngine UI Vertex Shader
 */

struct VertexAttribs
{
	float4 position : POSITION;
	float4 color : COLOR;
	float2 UV : TEXCOORD0;
};

VertexAttribs
main(float4 aPosUV : POSITION,
	float4 aColor : COLOR,
	uniform float4x4 uMVP)
{
	VertexAttribs ret;

	ret.position = mul(uMVP, float4(aPosUV.xy, 0.0, 1.0));
	ret.color = aColor;
	ret.UV = aPosUV.zw;

	return ret;
}