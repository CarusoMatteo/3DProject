#include "../../Header Files/Model/Buffers.h"
#include "../../Header Files/Model/Mesh.h"
#include "../../Header Files/Model/MeshFactory.h"
#include "../../Header Files/Model/ShaderFiles.h"
#include "../../Header Files/Model/Transform.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

using namespace std;
using namespace glm;

shared_ptr<Mesh> MeshFactory::cube(float length, ShaderFiles files, Transform transform)
{
	vector<fvec3> vertices;
	vector<fvec4> colors;
	vector<fvec3> normals;
	vector<unsigned int> indices;
	vector<fvec2> textures;
	const fvec3 anchor = fvec3(0);

	const float halfLength = length / 2.0f;

	vertices.push_back(fvec3(-halfLength, -halfLength, halfLength));
	colors.push_back(fvec4(1.0, 0.0, 0.0, 1));
	vertices.push_back(fvec3(halfLength, -halfLength, halfLength));
	colors.push_back(fvec4(0.0, 1.0, 0.0, 1));
	vertices.push_back(fvec3(halfLength, halfLength, halfLength));
	colors.push_back(fvec4(0.0, 0.0, 1.0, 1));
	vertices.push_back(fvec3(-halfLength, halfLength, halfLength));
	colors.push_back(fvec4(1.0, 0.0, 1.0, 1.0));
	// back
	vertices.push_back(fvec3(-halfLength, -halfLength, -halfLength));
	colors.push_back(fvec4(1.0, 1.0, 1.0, 1.0));
	vertices.push_back(fvec3(halfLength, -halfLength, -halfLength));
	colors.push_back(fvec4(1.0, 1.0, 1.0, 1.0));
	vertices.push_back(fvec3(halfLength, halfLength, -halfLength));
	colors.push_back(fvec4(1.0, 1.0, 1.0, 1.0));
	vertices.push_back(fvec3(-halfLength, halfLength, -halfLength));
	colors.push_back(fvec4(1.0, 1.0, 1.0, 1.0));

	// Indica come i vertici sono legati tre a tre.
	// Sono 12 terne che definiscono i 12 triangoli che compongono le 6 facce del cubo.
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(0);

	indices.push_back(1);
	indices.push_back(5);
	indices.push_back(6);

	indices.push_back(6);
	indices.push_back(2);
	indices.push_back(1);

	indices.push_back(7);
	indices.push_back(6);
	indices.push_back(5);

	indices.push_back(5);
	indices.push_back(4);
	indices.push_back(7);

	indices.push_back(4);
	indices.push_back(0);
	indices.push_back(3);

	indices.push_back(3);
	indices.push_back(7);
	indices.push_back(4);

	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(1);

	indices.push_back(1);
	indices.push_back(0);
	indices.push_back(4);

	indices.push_back(3);
	indices.push_back(2);
	indices.push_back(6);

	indices.push_back(6);
	indices.push_back(7);
	indices.push_back(3);

	transform.anchorPoint = anchor;
	// Add the anchor to indices?
	// vertices.push_back(anchor);
	// colors.push_back(fvec4(0.0, 1.0, 0.0, 1.0));
	// indices.push_back(nv - 1);

	const BufferValues bufferValues = {
		vertices,
		colors,
		normals,
		indices,
		textures};

	return shared_ptr<Mesh>(new Mesh(bufferValues, files, transform));
}