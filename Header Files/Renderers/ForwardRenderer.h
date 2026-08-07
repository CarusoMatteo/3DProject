#pragma once

#include "../Texture/Texture.h"
#include "Buffers.h"
#include "Renderer.h"
#include "Uniforms.h"
#include <memory>
#include <optional>

using namespace std;
using namespace glm;

struct Material;
struct ShaderFiles;
struct Transform;

class ForwardRenderer : public Renderer
{
public:
	ForwardRenderer(const ShaderFiles files);
	~ForwardRenderer();

	void setBufferValues(const ForwardBufferValues bufferValues);
	void render(const Transform modelTransform, const Transform meshTransform, const Material material, const optional<shared_ptr<Texture>> texture) override;

protected:
	unsigned int programId;
	ForwardBufferAddresses addresses;
	ForwardBufferValues values;
	ForwardUniforms uniforms;

	void initVao();
	void initVbos();
	void initUniformReferences();

	void updateUniformValues(const Transform modelTransform, const Transform meshTransform, const Material material, const optional<shared_ptr<Texture>> texture);
	void passUniforms();
	void bindTexture(const Texture texture) const;
	// Unbinds a texture from the shader. Used when the mesh has no texture.
	void bindNoTexture() const;
	void draw() const;
};
