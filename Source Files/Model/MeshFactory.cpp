#include "../../Header Files/Game Objects/Material.h"
#include "../../Header Files/Model/Buffers.h"
#include "../../Header Files/Model/Mesh.h"
#include "../../Header Files/Model/MeshFactory.h"
#include "../../Header Files/Model/ShaderFiles.h"
#include "../../Header Files/Model/Transform.h"
#include <cmath>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

using namespace std;
using namespace glm;

shared_ptr<Mesh> MeshFactory::plane(const string name, const fvec2 size, const ShaderFiles files, Transform transform)
{
	const fvec2 halfSize = size / 2.0f;
	const Material material = {
		"Snow White",
		fvec3(0.2, 0.2, 0.2),
		fvec3(0.95, 0.96, 0.98),
		fvec3(0.8, 0.8, 0.8),
		1.78125f};

	transform.anchorPoint = fvec3(0);
	const vector<fvec3> vertices = {
		fvec3(-halfSize.x, 0, halfSize.y),
		fvec3(halfSize.x, 0, halfSize.y),
		fvec3(halfSize.x, 0, -halfSize.y),
		fvec3(-halfSize.x, 0, -halfSize.y),
		// Anchor
		transform.anchorPoint};
	const vector<fvec4> colors = {
		fvec4(1),
		fvec4(1),
		fvec4(1),
		fvec4(1),
		// Anchor
		fvec4(1)};
	const vector<unsigned int> indices = {
		0, 1, 2,
		0, 2, 3,
		// Anchor
		4};
	const vector<fvec3> normals = {
		fvec3(0, 1, 0),
		fvec3(0, 1, 0),
		fvec3(0, 1, 0),
		fvec3(0, 1, 0)};
	const vector<fvec2> textures; // Not set

	const BufferValues bufferValues = {
		vertices,
		colors,
		normals,
		indices,
		textures};

	return shared_ptr<Mesh>(new Mesh(name, bufferValues, files, transform, material));
}

shared_ptr<Mesh> MeshFactory::cube(const string name, const float length, const ShaderFiles files, Transform transform)
{
	const float halfLength = length / 2.0f;
	const Material material = {
		"Red Plastic",
		fvec3(0.1, 0.0, 0.0),
		fvec3(0.6, 0.1, 0.1),
		fvec3(0.7, 0.6, 0.6),
		15000.0f};

	transform.anchorPoint = fvec3(0);
	const vector<fvec3> vertices = {
		// Front
		fvec3(-halfLength, -halfLength, halfLength),
		fvec3(halfLength, -halfLength, halfLength),
		fvec3(halfLength, halfLength, halfLength),
		fvec3(-halfLength, halfLength, halfLength),
		// Back
		fvec3(-halfLength, -halfLength, -halfLength),
		fvec3(halfLength, -halfLength, -halfLength),
		fvec3(halfLength, halfLength, -halfLength),
		fvec3(-halfLength, halfLength, -halfLength),
		// Anchor
		transform.anchorPoint};
	const vector<fvec4> colors = {
		// Front
		fvec4(1, 0, 0, 1),
		fvec4(0, 1, 0, 1),
		fvec4(0, 0, 1, 1),
		fvec4(1, 0, 1, 1),
		// Back
		fvec4(1),
		fvec4(1),
		fvec4(1),
		fvec4(1),
		// Anchor
		fvec4(1)};
	const vector<unsigned int> indices = {
		// Show how the vertices are connected three by three, with 12 triplets.
		0, 1, 2,
		2, 3, 0,
		1, 5, 6,
		6, 2, 1,
		7, 6, 5,
		5, 4, 7,
		4, 0, 3,
		3, 7, 4,
		4, 5, 1,
		1, 0, 4,
		3, 2, 6,
		6, 7, 3,
		// Anchor
		8};
	const vector<fvec3> normals = {
		// Front
		normalize(vec3(0, 0, 1)),
		normalize(vec3(0, 0, 1)),
		normalize(vec3(0, 0, 1)),
		normalize(vec3(0, 0, 1)),
		// Back
		normalize(vec3(0, 0, -1)),
		normalize(vec3(0, 0, -1)),
		normalize(vec3(0, 0, -1)),
		normalize(vec3(0, 0, -1))};
	const vector<fvec2> textures; // Not set

	const BufferValues bufferValues = {
		vertices,
		colors,
		normals,
		indices,
		textures};

	return shared_ptr<Mesh>(new Mesh(name, bufferValues, files, transform, material));
}

shared_ptr<Mesh> MeshFactory::sphere(const string name, const fvec3 radius, const ShaderFiles files, Transform transform)
{
	// Number of subdivisions along the y axis
	const int stacks = 100;
	// Number of subdivisions along the x axis
	const int slices = 100;
	const fvec4 color = fvec4(1.0, 0.0, 0.0, 1.0); // Tecnically not used if we use the Phong illumination shader.
	const Material material = {
		"Red Plastic",
		fvec3(0.1, 0.0, 0.0),
		fvec3(0.6, 0.1, 0.1),
		fvec3(0.7, 0.6, 0.6),
		150.0f};

	transform.anchorPoint = fvec3(0);
	vector<fvec3> vertices;
	vector<fvec4> colors;
	vector<fvec3> normals;
	vector<unsigned int> indices;
	vector<fvec2> textures; // Not set

	for (int i = 0; i <= stacks; i++)
	{
		const float V = i / static_cast<float>(stacks);
		const float phi = V * pi<float>();

		// Loop through slices
		for (int j = 0; j <= slices; j++)
		{
			const float U = j / static_cast<float>(slices);
			const float theta = U * 2 * pi<float>();

			// Find the vertex position
			const float x = radius.x * (cosf(theta) * sinf(phi));
			const float y = radius.y * cosf(phi);
			const float z = radius.z * sinf(theta) * sinf(phi);

			vertices.push_back(fvec3(x, y, z));
			colors.push_back(color);
			normals.push_back(fvec3(x, y, z));
		}
	}

	// Find the index
	for (int i = 0; i < slices * stacks + slices; i++)
	{
		indices.push_back(i);
		indices.push_back(i + slices + 1);
		indices.push_back(i + slices);
		indices.push_back(i + slices + 1);
		indices.push_back(i);
		indices.push_back(i + 1);
	}

	vertices.push_back(transform.anchorPoint);
	colors.push_back(fvec4(1));
	indices.push_back(static_cast<unsigned int>(vertices.size() - 1));

	const BufferValues bufferValues = {
		vertices,
		colors,
		normals,
		indices,
		textures};

	return shared_ptr<Mesh>(new Mesh(name, bufferValues, files, transform, material));
}

// Consider adding anchor to the vertex and index vector
// mesh->vertices.push_back(anchor); // Memorizzo come ultimo vertice l'ancora per poterla visualizzare
// mesh->colors.push_back(anchorColor);
// mesh->ancora_obj = vec4(anchor, 1.0); // memorizzo l'ancora nel campo ancora_obj
// mesh->indices.push_back(nv - 1);
