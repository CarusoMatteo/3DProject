#include "../../Header Files/Model/MeshFactory.h"
#include "../../Header Files/Model/Material.h"
#include "../../Header Files/Model/Mesh.h"
#include "../../Header Files/Model/Transform.h"
#include "../../Header Files/Renderers/Buffers.h"
#include "../../Header Files/Renderers/ForwardRenderer.h"
#include "../../Header Files/Renderers/GeometryRenderer.h"
#include "../../Header Files/Renderers/Renderer.h"
#include "../../Header Files/Texture/Texture.h"
#include <cmath>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;
using namespace glm;

bool isForwardRenderer(const shared_ptr<Renderer> shader)
{
	// Check if the shader is a ForwardRenderer by dynamic casting it.
	return dynamic_pointer_cast<ForwardRenderer>(shader) != nullptr;
}

bool isGeometryRenderer(const shared_ptr<Renderer> shader)
{
	// Check if the shader is a DeferredRenderer by dynamic casting it.
	return dynamic_pointer_cast<GeometryRenderer>(shader) != nullptr;
}

shared_ptr<Mesh> MeshFactory::plane(const string name, const fvec2 size, const shared_ptr<Renderer> shader, Transform transform, const optional<shared_ptr<Texture>> texture)
{
	const fvec2 halfSize = size / 2.0f;
	const Material material = MaterialsFactory::makeMaterial(MaterialType::SNOW_WHITE);

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
	const vector<fvec2> textureCoordinates = {
		fvec2(0, 1),
		fvec2(1, 1),
		fvec2(1, 0),
		fvec2(0, 0)};

	if (isForwardRenderer(shader))
	{
		const ForwardBufferValues bufferValues = {vertices, colors, normals, indices, textureCoordinates};
		dynamic_pointer_cast<ForwardRenderer>(shader)->setBufferValues(bufferValues);
	}
	else if (isGeometryRenderer(shader))
	{
		const GeometryBufferValues bufferValues = {/* TODO: positions, albedosSpecular, normals, depths */};
		dynamic_pointer_cast<GeometryRenderer>(shader)->setBufferValues(bufferValues);
	}
	else
	{
		cerr << "Error: Unknown shader type. Cannot set buffer values." << endl;
		throw runtime_error("Unknown shader type. Cannot set buffer values.");
	}

	return shared_ptr<Mesh>(new Mesh(name, shader, transform, material, texture));
}

shared_ptr<Mesh> MeshFactory::cube(const string name, const float length, const shared_ptr<Renderer> shader, Transform transform, const optional<shared_ptr<Texture>> texture)
{
	const float halfLength = length / 2.0f;
	const Material material = MaterialsFactory::makeMaterial(MaterialType::RED_PLASTIC);

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
	const vector<fvec2> textureCoordinates = {
		fvec2(0, 0),
		fvec2(1, 0),
		fvec2(1, 1),
		fvec2(0, 1),
		fvec2(0, 0),
		fvec2(1, 0),
		fvec2(1, 1),
		fvec2(0, 1)};

	if (isForwardRenderer(shader))
	{
		const ForwardBufferValues bufferValues = {vertices, colors, normals, indices, textureCoordinates};
		dynamic_pointer_cast<ForwardRenderer>(shader)->setBufferValues(bufferValues);
	}
	else if (isGeometryRenderer(shader))
	{
		const GeometryBufferValues bufferValues = {/* TODO: positions, albedosSpecular, normals, depths */};
		dynamic_pointer_cast<GeometryRenderer>(shader)->setBufferValues(bufferValues);
	}
	else
	{
		cerr << "Error: Unknown shader type. Cannot set buffer values." << endl;
		throw runtime_error("Unknown shader type. Cannot set buffer values.");
	}

	return shared_ptr<Mesh>(new Mesh(name, shader, transform, material, texture));
}

shared_ptr<Mesh> MeshFactory::sphere(const string name, const fvec3 radius, const shared_ptr<Renderer> shader, Transform transform, const optional<shared_ptr<Texture>> texture)
{
	// Number of subdivisions along the y axis
	const int stacks = 100;
	// Number of subdivisions along the x axis
	const int slices = 100;
	const fvec4 color = fvec4(1.0, 0.0, 0.0, 1.0);
	const Material material = MaterialsFactory::makeMaterial(MaterialType::SNOW_WHITE);

	transform.anchorPoint = fvec3(0);
	vector<fvec3> vertices;
	vector<fvec4> colors;
	vector<fvec3> normals;
	vector<unsigned int> indices;
	vector<fvec2> textureCoordinates;

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
			textureCoordinates.push_back(fvec2(U, V));
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

	if (isForwardRenderer(shader))
	{
		const ForwardBufferValues bufferValues = {vertices, colors, normals, indices, textureCoordinates};
		dynamic_pointer_cast<ForwardRenderer>(shader)->setBufferValues(bufferValues);
	}
	else if (isGeometryRenderer(shader))
	{
		const GeometryBufferValues bufferValues = {/* TODO: positions, albedosSpecular, normals, depths */};
		dynamic_pointer_cast<GeometryRenderer>(shader)->setBufferValues(bufferValues);
	}
	else
	{
		cerr << "Error: Unknown shader type. Cannot set buffer values." << endl;
		throw runtime_error("Unknown shader type. Cannot set buffer values.");
	}

	return shared_ptr<Mesh>(new Mesh(name, shader, transform, material, texture));
}

shared_ptr<Mesh> MeshFactory::screenQuad(shared_ptr<ForwardRenderer> shader)
{
	const vector<fvec3> vertices = {
		fvec3(-1, 1, 0),
		fvec3(-1, -1, 0),
		fvec3(1, 1, 0),
		fvec3(1, -1, 0)};
	const vector<fvec2> textureCoordinates = {
		fvec2(0, 1),
		fvec2(0, 0),
		fvec2(1, 1),
		fvec2(1, 0)};

	const ForwardBufferValues bufferValues = {vertices, {}, {}, {}, textureCoordinates};
	shader->setBufferValues(bufferValues);

	return shared_ptr<Mesh>(new Mesh("ScreenQuad", shader, Transform(), Material(), nullopt));
}

// Consider adding anchor to the vertex and index vector
// mesh->vertices.push_back(anchor); // Save it as the lsat vertex to show it
// mesh->colors.push_back(anchorColor);
// mesh->anchor_obj = vec4(anchor, 1.0); // Save the anchor in the anchor_obj field
// mesh->indices.push_back(nv - 1);
