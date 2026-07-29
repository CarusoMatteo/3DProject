#version 330 core
in vec4 color;
in vec2 vTexCoord;

uniform sampler2D textureSampler;
uniform bool useTexture;

out vec4 fragColor;

void main()
{
	if (useTexture)
	{
		color *= texture(textureSampler, vTexCoord);
	}

	fragColor = color;
}
