#pragma once

#include "../Game Objects/Material.h"
#include "Buffers.h"
#include "Transform.h"
#include <memory>
#include <string>

using namespace glm;
using namespace std;

class Shader;

class Mesh
{
public:
	Mesh(const string name, const BufferValues values, const shared_ptr<Shader> shader, const Transform transform, const Material material);
	~Mesh() = default;

	void render(Transform modelTransform) const;

private:
	string name;
	shared_ptr<Shader> shader;
	BufferValues values;
	Transform transform;
	// ShadingType shadingType;
	Material material;
};
