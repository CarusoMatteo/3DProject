#pragma once

#include "../Texture/Texture.h"
#include "BuffersAddresses.h"
#include "BuffersValues.h"
#include "Renderer.h"
#include "Uniforms.h"
#include <memory>
#include <optional>

using namespace std;
using namespace glm;

struct Material;
struct ShaderFiles;
struct Transform;

class ForwardRendererMRT : public Renderer
{
public:
	ForwardRendererMRT(const ShaderFiles files);
	~ForwardRendererMRT();

	void setBufferValues(const ForwardBufferValues bufferValues);
	void render(const float currentTime, const Transform modelTransform, const Transform meshTransform, const Material material, const optional<shared_ptr<Texture>> texture) override;

protected:
	unsigned int programId;
	ForwardBufferAddresses addresses;
	ForwardBufferValues values;
	ForwardUniforms uniforms;

	unsigned int fbo = -1;
	unsigned int texColor = -1;
	unsigned int texFileColor = -1;
	unsigned int texNormal = -1;
	unsigned int texDepth = -1;
	unsigned int rboDepth = -1;

	void initVao();
	void initVbos();
	void initUniformReferences();

	void updateUniformValues(const Transform modelTransform, const Transform meshTransform, const Material material, const optional<shared_ptr<Texture>> texture);
	void passUniforms();
	void bindTexture(const Texture texture) const;
	// Unbinds a texture from the shader. Used when the mesh has no texture.
	void bindNoTexture() const;
	void draw() const;
	void saveBuffers(const float currentTime, const ivec2 size);
};
