#version 330 core
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec4 vertexColor;
layout (location = 3) in vec2 vertexTextureCoordinate;

out vec4 color;
out vec2 fragmentTextureCoordinate;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
	color = vertexColor;
	fragmentTextureCoordinate = vertexTextureCoordinate;
}
