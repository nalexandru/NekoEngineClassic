/*
 * NekoEngine UI Vertex Shader
 */

attribute vec4 aPosUV;
attribute vec4 aColor;

uniform mat4 uMVP;

varying vec4 vColor;
varying vec2 vUV;

void
main()
{
	vColor = aColor;
	vUV = aPosUV.zw;
	
	gl_Position = uMVP * vec4(aPosUV.xy, 0.0, 1.0);
}
