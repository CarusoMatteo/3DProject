#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

out vec4 outColor;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
	outColor = color;
}
