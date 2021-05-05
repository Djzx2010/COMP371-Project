#version 330 core
in vec3 vertexColor;
in vec2 vertexUV;
in vec3 vertexNormal;
in vec3 FragPos;

uniform sampler2D textureSampler;
uniform sampler2D shadow_map;
uniform vec3 lightPos0;
uniform vec3 viewPos;
uniform int isTextured = 1;

uniform float light_cutoff_outer;
uniform float light_cutoff_inner;
uniform float light_near_plane;
uniform float light_far_plane;

out vec4 FragColor;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambientLight = ambientStrength * vec3(1.0f, 1.0f, 1.0f);
	
	vec3 norm = normalize(vertexNormal);
	vec3 lightDir = normalize(lightPos0 - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuseColor = vec3(1.0f, 1.0f, 1.0f);
	vec3 diffuse = diff * diffuseColor;
	
	float specularStrength = 0.5f;

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specularColor = vec3(1.0f, 1.0f, 1.0f);
	vec3 specular = specularStrength * spec * specularColor;
	
	vec4 textureColor = texture( textureSampler, vertexUV );
	if(isTextured==1){
		FragColor = textureColor * (vec4(ambientLight, 1.0f) + vec4(diffuse, 1.0f) + vec4(specular, 1.0f));
	}
	else{
		FragColor = vec4(vertexColor,1.0f) * (vec4(ambientLight, 1.0f) + vec4(diffuse, 1.0f) + vec4(specular, 1.0f));
	}
}