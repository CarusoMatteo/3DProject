#version 330 core
in vec4 color;
in vec2 textureCoordinate;

uniform sampler2D textureSampler;
uniform bool useTexture;

out vec4 fragColor;

void main()
{
	fragColor = color;

	if (useTexture)
	{
		fragColor *= texture(textureSampler, textureCoordinate);
	}
}
