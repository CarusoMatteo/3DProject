#version 330 core

in vec3 N, L, R, V;
in vec2 textureCoordinate;

uniform sampler2D textureSampler;
uniform bool useTexture;

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
out vec4 fragColor;

const float strenght = 0.1;
void main()
{
	vec3 halfwayVector = normalize(L + V);
	vec3 ambient = strenght * light.power * material.ambient;
	vec3 diffuse = light.power * light.color * max(dot(L, N), 0) * material.diffuse;
	vec3 specular = light.power * light.color * pow(max(dot(N, halfwayVector), 0), material.shininess) * material.specular;

	vec4 baseColor = vec4(ambient + diffuse + specular, 1.0);
	if (useTexture)
	{
		baseColor *= texture(textureSampler, textureCoordinate);
	}

	fragColor = baseColor;
}
