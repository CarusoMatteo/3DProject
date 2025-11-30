#pragma once

#include "Transform.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

using namespace glm;
using namespace std;

class Shader;
struct ShaderFiles;

class Mesh
{
public:
	Mesh(const vector<fvec3> vertices, const vector<fvec4> colors, const vector<unsigned int> indices, const ShaderFiles files, const Transform transform);
	~Mesh() = default;

	void render();

private:
	vector<fvec3> vertices;
	vector<fvec4> colors;
	vector<unsigned int> indices;
	vector<fvec3> normals;
	vector<fvec2> textureCoords;

	Transform transform;

	string name;
	unique_ptr<Shader> shader;
};
