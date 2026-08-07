#pragma once

#include "../Texture/Texture.h"
#include "Buffers.h"
#include "Renderer.h"
#include "Uniforms.h"
#include <memory>
#include <optional>

using namespace std;
using namespace glm;

// struct Material;
struct ShaderFiles;
// struct Transform;

class DeferredRenderer : public Renderer
{
public:
	DeferredRenderer(const ShaderFiles files);
	~DeferredRenderer();

	void setBufferValues(const DeferredBufferValues bufferValues);
	void render(const Transform modelTransform, const Transform meshTransform, const Material material, const optional<shared_ptr<Texture>> texture) override;

protected:
	unsigned int programId;
	DeferredBuffersAddresses addresses;
	DeferredBufferValues values;
	DeferredUniforms uniforms;

	void initGBuffer();
	void initSubBuffers();
	void initDepthRenderBuffer();
	void initUniformReferences();

	void updateUniformValues(const Transform modelTransform, const Transform meshTransform, const Material material, const optional<shared_ptr<Texture>> texture);
	void passUniforms();
	void draw() const;
};
