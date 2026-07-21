#pragma once

#include "../Game Objects/Material.h"
#include "Buffers.h"
#include "Texture.h"
#include "Transform.h"
#include <memory>
#include <string>

using namespace glm;
using namespace std;

class Shader;

class CubeMapMesh
{
public:
	CubeMapMesh(const string name, const shared_ptr<Shader> shader, const Transform transform, const Texture texture);
	~CubeMapMesh() = default;

	void render(Transform modelTransform) const;
	string getName() const;

	void setShader(const shared_ptr<Shader> shader);

	MaterialType getMaterialType() const;
	void setMaterialType(const MaterialType material);
	shared_ptr<Material> getCustomMaterial() const;

	void setTexture(const Texture texture);
	Texture getTexture() const;

	Transform getTransform() const;
	void setTransform(const Transform transform);

private:
	string name;
	shared_ptr<Shader> shader;
	BufferValues values;
	Transform transform;
	MaterialType material;
	shared_ptr<Material> customMaterial;
	Texture texture;
};
