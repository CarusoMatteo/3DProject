#pragma once

#include "../Game Objects/Material.h"
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
	Mesh(const string name, const BufferValues values, const ShaderFiles files, const Transform transform, const Material material);
	~Mesh() = default;

	void render(Transform modelTransform) const;

private:
	string name;
	unique_ptr<Shader> shader;
	BufferValues values;
	Transform transform;
	// ShadingType shadingType;
	Material material;
};
