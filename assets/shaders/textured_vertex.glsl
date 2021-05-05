#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aNormal;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 projectionMatrix = mat4(1.0);
uniform mat4 light_view_proj_matrix;

out vec3 vertexColor;
out vec2 vertexUV;
out vec3 vertexNormal;
out vec3 FragPos;
out vec3 fragment_normal;
out vec3 fragment_position;
out vec4 fragment_position_light_space;

void main()
{
	vertexColor = aColor;
	mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;
	gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	vertexUV = aUV;
	FragPos = vec3(worldMatrix * vec4(aPos, 1.0));
	vertexNormal = mat3(transpose(inverse(worldMatrix))) * aNormal;
	fragment_normal = mat3(modelViewProjection) * aNormal;
	fragment_position = vec3(modelViewProjection * vec4(aPos, 1.0));
	fragment_position_light_space = light_view_proj_matrix * vec4(fragment_position, 1.0);
}