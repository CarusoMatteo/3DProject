#version 330 core
layout (location = 0) in vec3 vertexPosition;
layout (location = 2) in vec3 vertexNormal;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

uniform mat4 projectionMatrixPrev;
uniform mat4 modelMatrixPrev;
uniform mat4 viewMatrixPrev;

out vec3 normal, N;
out vec3 position;

out vec4 clipPosition;
out vec4 clipPositionPrev;

void main()
{
	clipPosition = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
	clipPositionPrev = projectionMatrixPrev * viewMatrixPrev * modelMatrixPrev * vec4(vertexPosition, 1.0);
	gl_Position = clipPosition;

	// Normals in world coordinates
	normal = mat3(transpose(inverse(modelMatrix))) * vertexNormal;
	// Normals in view coordinates
	N = mat3(transpose(inverse(viewMatrix * modelMatrix))) * vertexNormal;
	// Position in world coordinates
	position = vec3(modelMatrix * vec4(vertexPosition, 1.0));
}
