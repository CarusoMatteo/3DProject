#include "../../Header Files/Model/Buffers.h"
#include "../../Header Files/Model/Mesh.h"
#include "../../Header Files/Model/MeshFactory.h"
#include "../../Header Files/Model/ShaderFiles.h"
#include "../../Header Files/Model/Transform.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

using namespace std;
using namespace glm;

shared_ptr<Mesh> MeshFactory::plane(const string name, const fvec2 size, const ShaderFiles files, Transform transform)
{
	const fvec2 halfSize = size / 2.0f;

	transform.anchorPoint = fvec3(0);
	const vector<fvec3> vertices = {
		vec3(-halfSize.x, 0, halfSize.y),
		vec3(halfSize.x, 0, halfSize.y),
		vec3(halfSize.x, 0, -halfSize.y),
		vec3(-halfSize.x, 0, -halfSize.y)};
	const vector<fvec4> colors = {
		fvec4(1),
		fvec4(1),
		fvec4(1),
		fvec4(1)};
	const vector<unsigned int> indices = {
		0, 1, 2,
		2, 3, 0};
	const vector<fvec3> normals = {
		vec3(0, 1, 0),
		vec3(0, 1, 0),
		vec3(0, 1, 0),
		vec3(0, 1, 0)};
	const vector<fvec2> textures; // Not set

	const BufferValues bufferValues = {
		vertices,
		colors,
		normals,
		indices,
		textures};

	return shared_ptr<Mesh>(new Mesh(name, bufferValues, files, transform));
}

shared_ptr<Mesh> MeshFactory::cube(const string name, const float length, const ShaderFiles files, Transform transform)
{
	const float halfLength = length / 2.0f;

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
		fvec3(-halfLength, halfLength, -halfLength)};
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
		fvec4(1)};
	const vector<fvec3> normals; // Not set
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
		6, 7, 3};
	const vector<fvec2> textures; // Not set

	const BufferValues bufferValues = {
		vertices,
		colors,
		normals,
		indices,
		textures};

	return shared_ptr<Mesh>(new Mesh(name, bufferValues, files, transform));
}

// Consider adding anchor to the vertex and index vector
// mesh->vertices.push_back(anchor); // Memorizzo come ultimo vertice l'ancora per poterla visualizzare
// mesh->colors.push_back(anchorColor);
// mesh->ancora_obj = vec4(anchor, 1.0); // memorizzo l'ancora nel campo ancora_obj
// mesh->indices.push_back(nv - 1);
