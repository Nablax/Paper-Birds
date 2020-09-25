#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 normal;
in vec3 FragPos;  

#define backFace false
#define frontFace true

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform bool renderFace;

void main()
{
	vec4 texColor = texture(texture_diffuse1, TexCoords);
	if(texColor.a < 0.7)
		discard;

	const float gamma = 2.2;
	vec3 ambient = 0.3f * texColor.rgb;

	vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - FragPos);
	float diff = dot(norm, lightDir);
	if(renderFace == backFace){
		diff = max(-diff, 0.0);
	}
	if(renderFace == frontFace){
		diff = max(diff, 0.0);
	}
    vec3 diffuse = diff * texColor.rgb;  
    vec3 res = (ambient + diffuse) * lightColor;

	res = pow(res, vec3(1.0 / gamma));
    FragColor = vec4(res, 1.0f);
}