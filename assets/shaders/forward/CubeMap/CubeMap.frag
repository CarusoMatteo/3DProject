#version 330 core
uniform samplerCube skyboxSampler;
uniform int renderMode;

in vec3 fragmentTextureCoordinate;

in vec4 clipPosition;
in vec4 clipPositionPrev;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 mainColor;
layout (location = 2) out vec4 normalColor;
layout (location = 3) out vec4 depthColor;
layout (location = 4) out vec4 velocityColor;

vec2 motionVector()
{
	// From clip space to normalized device coordinates (NDC)
	vec2 ndcCurrent = clipPosition.xy / clipPosition.w;
	vec2 ndcPrevious = clipPositionPrev.xy / clipPositionPrev.w;
	// Motion vector in NDC space
	vec2 motionVector = ndcCurrent - ndcPrevious;
	// Convert to color space [-1, 1] => [0, 1]
	return motionVector * 0.5 + 0.5;
	// return motionVector * 2 + 0.5;
}

void main()
{
	mainColor = texture(skyboxSampler, fragmentTextureCoordinate);
	normalColor = vec4(0, 0, 1, 1);
	depthColor = vec4(1.0);
	velocityColor = vec4(motionVector(), 0, 1);

	if (renderMode == 2)
		fragColor = normalColor;
	else if (renderMode == 3)
		fragColor = depthColor;
	else if (renderMode == 4)
		fragColor = velocityColor;
	else if (renderMode == 1)
		fragColor = mainColor;
}
