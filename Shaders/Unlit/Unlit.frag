#version 330 core
uniform bool useTexture;
uniform sampler2D textureSampler;

in vec4 color;
in vec2 textureCoordinate;

out vec4 fragColor;

void main()
{
	fragColor = color;

	if (useTexture)
	{
		fragColor *= texture(textureSampler, textureCoordinate);
	}
}