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

class GeometryRenderer : public Renderer
{
public:
	GeometryRenderer(const ShaderFiles files);
	~GeometryRenderer();

	void setBufferValues(const GeometryBufferValues bufferValues);
	void render(const Transform modelTransform, const Transform meshTransform, const Material material, const optional<shared_ptr<Texture>> texture) override;

protected:
	unsigned int programId;
	GeometryBuffersAddresses addresses;
	GeometryBufferValues values;
	GeometryUniforms uniforms;

	void initGBuffer();
	void initSubBuffers();
	void initDepthRenderBuffer();
	void initUniformReferences();

	void updateUniformValues(const Transform modelTransform, const Transform meshTransform, const Material material, const optional<shared_ptr<Texture>> texture);
	void passUniforms();
	void draw() const;
};
