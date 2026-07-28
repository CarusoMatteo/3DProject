#version 330 core

uniform sampler2D textureSampler;
uniform bool useTexture;

in vec3 N, L, R, V;
in vec2 textureCoordinate;

out vec4 fragColor;

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

void main()
{
	vec3 ambient = strength * light.power * material.ambient;
	vec3 diffuse = light.power * light.color * max(dot(L, N), 0) * material.diffuse;
	vec3 specular = light.power * light.color * pow(max(dot(V, R), 0), material.shininess) * material.specular;
	vec4 baseColor = vec4(ambient + diffuse + specular, 1.0);

	if (useTexture)
	{
		baseColor *= texture(textureSampler, textureCoordinate);
	}

	fragColor = baseColor;
}
