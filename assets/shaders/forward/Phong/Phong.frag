#version 330 core

uniform sampler2D textureSampler;
uniform bool useTexture;

in vec3 N, L, R, V;
in vec2 fragmentTextureCoordinate;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 mainColor;
layout (location = 2) out vec4 normalColor;
layout (location = 3) out vec4 depthColor;
layout (location = 4) out vec4 velocityColor;

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

const float strength = 0.1;
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
	vec3 ambient = strength * light.power * material.ambient;
	vec3 diffuse = light.power * light.color * max(dot(L, N), 0) * material.diffuse;
	vec3 specular = light.power * light.color * pow(max(dot(V, R), 0), material.shininess) * material.specular;
	vec4 baseColor = vec4(ambient + diffuse + specular, 1.0);

	if (useTexture)
	{
		baseColor *= texture(textureSampler, fragmentTextureCoordinate);
	}

	fragColor = mainColor = baseColor;
	normalColor = vec4(N, 1);
	depthColor = vec4(vec3(linearizeDepth(gl_FragCoord.z)), 1);
}
