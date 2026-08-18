#version 330 core
in vec4 color;
in vec2 fragmentTextureCoordinate;
in vec3 N;

uniform sampler2D textureSampler;
uniform bool useTexture;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 mainColor;
layout (location = 2) out vec4 normalColor;
layout (location = 3) out vec4 depthColor;
layout (location = 4) out vec4 velocityColor;

const float nearPlane = 0.1;
const float farPlane = 75.0;

float linearizeDepth(float depth)
{
	// Transform the depth buffer from [0,1] to normalized device coordinates [-1,1]
	float z = depth * 2.0 - 1.0;
	// Apply the linearization formula
	return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane)) / farPlane;
}
void main()
{
	vec4 baseColor = color;
	if (useTexture)
	{
		baseColor *= texture(textureSampler, fragmentTextureCoordinate);
	}

	fragColor = mainColor = baseColor;
	normalColor = vec4(N, 1);
	depthColor = vec4(vec3(linearizeDepth(gl_FragCoord.z)), 1);
}
