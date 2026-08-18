#version 330 core
uniform samplerCube skyboxSampler;

in vec3 fragmentTextureCoordinate;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 mainColor;
layout (location = 2) out vec4 normalColor;
layout (location = 3) out vec4 depthColor;

void main()
{
	fragColor = mainColor = texture(skyboxSampler, fragmentTextureCoordinate);
	normalColor = vec4(0, 0, 1, 1);
	depthColor = vec4(0, 0, 0, 1);
}
