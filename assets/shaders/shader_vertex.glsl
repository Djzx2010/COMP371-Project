#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 vertexColor;
void main()
{
	vertexColor = aColor;
	mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;
	gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}