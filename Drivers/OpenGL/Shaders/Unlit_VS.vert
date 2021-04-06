/*
 * NekoEngine Unlit Material Shader
 */

attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec3 aTangent;
attribute vec2 aUV;

uniform mat4 uMVP;

varying vec2 vUV;

void
main()
{
	vUV = aUV;
	gl_Position = uMVP * vec4(aPosition, 1.0);
}
