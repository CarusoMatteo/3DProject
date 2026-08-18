#version 330 core
in vec4 color;
in vec2 fragmentTextureCoordinate;
in vec3 N

uniform sampler2D textureSampler;
uniform bool useTexture;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 mainColor;
layout (location = 2) out vec4 normalColor;
layout (location = 3) out vec4 depthColor;

void main()
{
	vec4 baseColor = color;
	if (useTexture)
	{
		baseColor *= texture(textureSampler, fragmentTextureCoordinate);
	}

	fragColor = mainColor = baseColor;
	normalColor = vec4(N, 1);
	depthColor = vec4(gl_FragCoord.zzz, 1);
}
