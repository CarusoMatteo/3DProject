#pragma once

#include "../Texture/Texture.h"
#include "Material.h"
#include "Transform.h"
#include <memory>
#include <optional>
#include <string>

using namespace glm;
using namespace std;

class Renderer;

class Mesh
{
public:
	Mesh(const string name, const shared_ptr<Renderer> shader, const Transform transform, const Material material, const optional<shared_ptr<Texture>> texture);
	~Mesh() = default;

	void render(Transform modelTransform) const;
	string getName() const;

	void setShader(const shared_ptr<Renderer> shader);

	Material getMaterial() const;
	void setMaterial(const Material material);

	void setTexture(const shared_ptr<Texture> texture);
	optional<shared_ptr<Texture>> getTexture() const;

	Transform getTransform() const;
	void setTransform(const Transform transform);

private:
	string name;
	shared_ptr<Renderer> shader;
	Transform transform;
	Material material;
	// A mesh may or may not have a texture.
	optional<shared_ptr<Texture>> texture;
};
