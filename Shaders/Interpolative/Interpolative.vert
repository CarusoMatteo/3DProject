#version 330 core
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec4 vertexColor;
layout (location = 2) in vec3 vertexNormal;
layout (location = 3) in vec2 vertexTextureCoordinate;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform vec3 viewPosition;

out vec4 color;
out vec2 fragmentTextureCoordinate;

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

const float strenght = 0.1;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);

	vec4 eyePosition = viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);

	vec4 eyeLightPos = viewMatrix * vec4(light.position, 1.0);

	vec3 N = normalize(transpose(inverse(mat3(viewMatrix * modelMatrix))) * vertexNormal);

	vec3 V = normalize(viewPosition - eyePosition.xyz);
	vec3 L = normalize((eyeLightPos - eyePosition).xyz);
	vec3 R = reflect(-L, N);

	vec3 ambient = strenght * light.power * material.ambient;
	vec3 diffuse = light.power * light.color * max(dot(L, N), 0) * material.diffuse;
	vec3 specular = light.power * light.color * pow(max(dot(V, R), 0), material.shininess) * material.specular;

	color = vec4(ambient + diffuse + specular, 1.0);

	fragmentTextureCoordinate = vertexTextureCoordinate;
}
