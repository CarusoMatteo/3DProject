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
	GeometryRenderer(const ShaderFiles geometryPassFiles, const ShaderFiles lightingPassFiles);
	~GeometryRenderer();

	void setBufferValues(const GeometryBufferValues bufferValues, const ForwardBufferValues forwardBufferValues);
	void render(const float currentTime, const Transform modelTransform, const Transform meshTransform, const Material material, const optional<shared_ptr<Texture>> texture) override;
	void finishGeometryPass();
	void lightingPass() const;

protected:
	unsigned int geometryPassProgramId;
	unsigned int lightingPassProgramId;

	GeometryBuffersAddresses addresses;
	GeometryBufferValues values;
	GeometryUniforms uniforms;

	ForwardBufferAddresses forwardAddresses;
	ForwardBufferValues forwardValues;

	SingleBufferAddresses quadAddresses;
	SingleBufferValues quadValues;

	optional<float> lastFrameTime = nullopt;

	// Init G-Buffer

	void initGBuffer();
	void initUniformReferences();

	// Init ForwardRenderer buffers
	void initVao();
	void initVbos();

	// Init ScreenQuad buffers
	void initQuad();

	void initFrameBuffer();

	void geometryPass(const float currentTime, const Transform modelTransform, const Transform meshTransform);
	void updateUniformValues(const Transform modelTransform, const Transform meshTransform);
	void passUniforms();
	void draw() const;
};
