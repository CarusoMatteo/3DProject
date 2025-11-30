#pragma once

#include "Buffers.h"
#include "Shader.h"
#include "Transform.h"
#include <memory>
#include <string>

using namespace glm;
using namespace std;

struct ShaderFiles;

class Mesh
{
public:
	Mesh(const BufferValues values, const ShaderFiles files, const Transform transform);
	~Mesh() = default;

	void render();

private:
	string name;
	unique_ptr<Shader> shader;
	BufferValues values;
	Transform transform;
};
