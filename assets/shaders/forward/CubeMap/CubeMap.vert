#version 330 core
layout (location = 0) in vec3 vertexPosition;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

uniform mat4 projectionMatrixPrev;
uniform mat4 modelMatrixPrev;
uniform mat4 viewMatrixPrev;

out vec3 fragmentTextureCoordinate;

out vec4 clipPosition;
out vec4 clipPositionPrev;

void main()
{
	fragmentTextureCoordinate = vertexPosition;

	// View Matrix without the translation component
	mat4 staticView = mat4(mat3(viewMatrix));
	clipPosition = projectionMatrix * staticView * vec4(vertexPosition, 1.0);
	clipPositionPrev = projectionMatrixPrev * staticView * vec4(vertexPosition, 1.0);
	gl_Position = clipPosition;
}
