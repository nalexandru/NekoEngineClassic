attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec3 aTangent;
attribute vec2 aUV;

//uniform mat4 u_mvp;

varying vec2 vUV;

void
main()
{
	vUV = aUV;

	//gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	
	gl_Position = vec4(aPosition, 1.0);
}
