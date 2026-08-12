#version 330 core
uniform bool useTexture;
uniform sampler2D textureSampler;

in vec4 color;
in vec2 fragmentTextureCoordinate;

out vec4 fragColor;

void main()
{
	// Ignore vertex colors and only use texture if it's available
	if (useTexture)
		fragColor = texture(textureSampler, fragmentTextureCoordinate);
	else
		fragColor = color;
}