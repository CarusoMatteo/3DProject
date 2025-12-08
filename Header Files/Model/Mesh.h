#pragma once

#include "../Game Objects/Material.h"
#include "../Game Objects/Texture.h"
#include "Buffers.h"
#include "Shader.h"
#include "Transform.h"
#include <memory>
#include <string>

using namespace glm;
using namespace std;

class Shader;

class Mesh
{
public:
	Mesh(const string name, const BufferValues values, const shared_ptr<Shader> shader, const Transform transform, const MaterialType material, const TextureType texture);
	~Mesh() = default;

	void render(Transform modelTransform) const;
	string getName() const;

	void setShader(const shared_ptr<Shader> shader);

	MaterialType getMaterialType() const;
	void setMaterialType(const MaterialType material);
	shared_ptr<Material> getCustomMaterial() const;

	void setTextureType(const TextureType texture);
	TextureType getTextureType() const;

	Transform getTransform() const;
	void setTransform(const Transform transform);

	ShaderType getShaderType() const;

private:
	string name;
	shared_ptr<Shader> shader;
	BufferValues values;
	Transform transform;
	MaterialType material;
	shared_ptr<Material> customMaterial;
	TextureType texture;
};
