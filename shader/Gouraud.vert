varying vec4 vertex_color;
uniform sampler2D tex1, tex2;

void main() {
	vec4 texel1;
	vec4 texel2;
	
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_MultiTexCoord1;
	
	vec3 v = vec3(gl_ModelViewMatrix * gl_Vertex);
	vec3 N = normalize(gl_NormalMatrix * gl_Normal);
	
	vec3 L = normalize(gl_LightSource[0].position.xyz - v);
	vec3 E = normalize(-v);//0-v
	vec3 R = normalize(-reflect(L, N));
	
	vec4 ambient = gl_FrontLightProduct[0].ambient * gl_FrontMaterial.ambient;
	vec4 diffuse = gl_FrontLightProduct[0].diffuse * max(dot(N, L), 0.0) * gl_FrontMaterial.diffuse;
	vec4 specular = gl_FrontLightProduct[0].specular * pow(max(dot(R, E), 0.0), gl_FrontMaterial.shininess) * gl_FrontMaterial.specular;
	
	texel1 = texture2D(tex1, gl_TexCoord[0].st);
    texel2 = texture2D(tex2, gl_TexCoord[1].st);
	
	vertex_color = (texel1 + texel2) / 2 + ambient + diffuse + specular;
}