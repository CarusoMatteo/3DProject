#version 330 core
uniform samplerCube skyboxSampler;

in vec3 fragmentTextureCoordinate;

out vec4 fragColor;

void main()
{
	fragColor = texture(skyboxSampler, fragmentTextureCoordinate);
}