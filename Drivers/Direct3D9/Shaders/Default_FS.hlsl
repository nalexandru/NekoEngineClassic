uniform sampler2D uDiffuseMap;
//uniform sampler2D u_normal;
//uniform sampler2D u_specular;

varying vec2 vUV;

uniform vec4 uDiffuseColor;

void
main()
{
	vec4 baseColor = uDiffuseColor * texture2D(uDiffuseMap, vUV);
	gl_FragColor = baseColor;
}
