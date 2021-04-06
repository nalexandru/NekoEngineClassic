/*
 * NekoEngine Text Fragment Shader
 */
 
uniform sampler2D uTextMap;

varying vec4 vColor;
varying vec2 vUV;

void
main()
{
	gl_FragColor = vec4(vColor.xyz, vColor.a * texture2D(uTextMap, vUV).r);
}
