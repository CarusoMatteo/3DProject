#version 330 core
layout (location = 0) in vec3 vertexPosition;
layout (location = 2) in vec3 vertexNormal;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec3 normal;
out vec3 position;

void main()
{
	// Normali in coordinate del mondo
	normal = mat3(transpose(inverse(modelMatrix))) * vertexNormal;
	// Posizione in coordinate del mondo
	position = vec3(modelMatrix * vec4(vertexPosition, 1.0));

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
	depthColor = vec4(vec3(linearizeDepth(gl_FragCoord.z)), 1);
}
