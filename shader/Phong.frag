varying vec3 N;
varying vec3 v;
uniform sampler2D tex1, tex2;

void main() {
	vec3 L = normalize(gl_LightSource[0].position.xyz - v);
	vec3 E = normalize(-v);//0-v
	vec3 R = normalize(-reflect(L, N));
	vec4 texel1;
	vec4 texel2;
	
	vec4 ambient = gl_FrontLightProduct[0].ambient * gl_FrontMaterial.ambient;
	vec4 diffuse = gl_FrontLightProduct[0].diffuse * max(dot(N, L), 0.0) * gl_FrontMaterial.diffuse;
	vec4 specular = gl_FrontLightProduct[0].specular * pow(max(dot(R, E), 0.0), gl_FrontMaterial.shininess) * gl_FrontMaterial.specular;
	
	texel1 = texture2D(tex1, gl_TexCoord[0].st);
    texel2 = texture2D(tex2, gl_TexCoord[1].st);
	
	gl_FragColor = (texel1 + texel2) / 2 + ambient + diffuse + specular;
}