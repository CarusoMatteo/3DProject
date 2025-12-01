#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec3 vertexNormal;

out vec4 outColor;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform vec3 viewPosition;

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
	// Transform the vertex coordinates from object space to world space.
	vec4 eyePosition = viewMatrix * modelMatrix * vec4(position, 1.0);

	// Project the world space coordinates into the canonical cube.
	gl_Position = projectionMatrix * eyePosition;

	// Transform the light position to view coordinates.
	vec4 eyeLightPos = viewMatrix * vec4(light.position, 1.0);

	// Transform the vertex normal to view coordinates.
	// Since normals are not points, we cannot do View*Model*Normal.
	vec3 N = normalize(mat3(transpose(inverse(viewMatrix * modelMatrix))) * vertexNormal);

	// Find light direction (light -> point)
	vec3 L = normalize(eyeLightPos.xyz - eyePosition.xyz);

	// Find view direction (view -> point)
	vec3 V = normalize(viewPosition - eyePosition.xyz);

	// Find reflection direction (reflection of L about N)
	vec3 R = reflect(-L, N);

	// I_a * k_a
	vec3 ambient = light.power * material.ambient;

	// Angle between light and normal
	float cos_theta = max(dot(N, L), 0);
	// I_d * k_d * cos(theta)
	vec3 diffuse = light.power * cos_theta * material.diffuse;

	// Angle between reflection and view
	float cos_alpha_n = pow(max(dot(V, R), 0), material.shininess); 
	// I_s * k_s * cos(alpha)^n
	vec3 specular = light.power * cos_alpha_n * material.specular; 

	// The original color is given by diffuse, no longer by aColor
	outColor = vec4(ambient + diffuse + specular, 1.0);
}
