/*
 * NekoEngine Unlit Material Shader
 */
 
uniform sampler2D uDiffuseMap;

varying vec2 vUV;

uniform vec4 uDiffuseColor;

void
main()
{
	gl_FragColor = uDiffuseColor * texture2D(uDiffuseMap, vUV);
}
