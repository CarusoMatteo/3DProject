#version 330 core
layout (location = 0) in vec3 vertexPosition;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
// Doesn't have a model matrix, so we don't pass it as a uniform.

out vec3 textureCoordinate;

void main()
{
	textureCoordinate = vertexPosition;

	// View Matrix without the translation component
	mat4 staticView = mat4(mat3(viewMatrix));
	gl_Position = projectionMatrix * staticView * vec4(vertexPosition, 1.0);
}
