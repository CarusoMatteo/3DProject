#version 330 core
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec4 vertexColor;
layout (location = 2) in vec3 vertexNormal;
// layout (location = 3) in vec2 vertexTextureCoordinate;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform vec3 viewPosition;

out vec3 L, N, R, V;
// out vec2 textureCoordinate;

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

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);

	vec4 eyePosition = viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);

	vec4 eyeLightPos = viewMatrix * vec4(light.position, 1.0);

	N = normalize(transpose(inverse(mat3(viewMatrix * modelMatrix))) * vertexNormal);

	V = normalize(viewPosition - eyePosition.xyz);
	L = normalize((eyeLightPos - eyePosition).xyz);
	R = reflect(-L, N);
	// textureCoordinate = vertexTextureCoordinate;
}
