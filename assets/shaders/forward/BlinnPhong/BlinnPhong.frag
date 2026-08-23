#version 330 core

in vec3 N, L, R, V;
in vec2 fragmentTextureCoordinate;

in vec4 clipPosition;
in vec4 clipPositionPrev;

uniform sampler2D textureSampler;
uniform bool useTexture;
uniform int renderMode;

struct PointLight
{
	vec3 position;
	vec3 color;
	float power;
};
uniform PointLight light;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 mainColor;
layout (location = 2) out vec4 normalColor;
layout (location = 3) out vec4 depthColor;
layout (location = 4) out vec4 velocityColor;

const float strength = 0.1;
const float nearPlane = 0.1;
const float farPlane = 100.0;

float linearizeDepth(float depth)
{
	// Transform the depth buffer from [0,1] to normalized device coordinates [-1,1]
	float z = depth * 2.0 - 1.0;
	// Apply the linearization formula
	float linearDepth = (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));

	// Normalize between farPlane and nearPlane
	// return (linearDepth - nearPlane) / (farPlane - nearPlane);

	// Logarithmic normalization, amplifies the depth differences for objects closer to the camera
	return log(linearDepth / nearPlane) / log(farPlane / nearPlane);
}

vec2 motionVector()
{
	// From clip space to normalized device coordinates (NDC)
	vec2 ndcCurrent = clipPosition.xy / clipPosition.w;
	vec2 ndcPrevious = clipPositionPrev.xy / clipPositionPrev.w;
	// Motion vector in NDC space
	vec2 motionVector = ndcCurrent - ndcPrevious;
	// Convert to color space [-1, 1] => [0, 1]
	return motionVector * 0.5 + 0.5;
}

void main()
{
	vec3 halfwayVector = normalize(L + V);
	vec3 ambient = strength * light.power * material.ambient;
	vec3 diffuse = light.power * light.color * max(dot(L, N), 0) * material.diffuse;
	vec3 specular = light.power * light.color * pow(max(dot(N, halfwayVector), 0), material.shininess) * material.specular;

	vec4 baseColor = vec4(ambient + diffuse + specular, 1.0);
	if (useTexture)
	{
		baseColor *= texture(textureSampler, fragmentTextureCoordinate);
	}

	if (baseColor.a < 0.5)
		discard;

	mainColor = baseColor;
	normalColor = vec4(N, 1);
	depthColor = vec4(vec3(linearizeDepth(gl_FragCoord.z)), 1);
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
