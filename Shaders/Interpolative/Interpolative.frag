#version 330 core
in vec4 color;
in vec2 fragmentTextureCoordinate;

uniform sampler2D textureSampler;
uniform bool useTexture;

out vec4 fragColor;

void main()
{
	vec4 baseColor = color;
	if (useTexture)
	{
		baseColor *= texture(textureSampler, fragmentTextureCoordinate);
	}

	fragColor = baseColor;
}
