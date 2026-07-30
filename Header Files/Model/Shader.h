#pragma once

#include "Buffers.h"
#include "IShader.h"
#include "Texture.h"
#include "Uniforms.h"
#include <memory>
#include <optional>

using namespace std;
using namespace glm;

struct Material;
struct ShaderFiles;
struct Transform;

class Shader : public IShader
{
public:
	Shader(const ShaderFiles files);
	~Shader();

	void setBufferValues(const BufferValues bufferValues) override;
	void render(const Transform modelTransform, const Transform meshTransform, const BufferValues values, const Material material, const optional<shared_ptr<Texture>> texture) override;

	static shared_ptr<bool> getDrawWireframeFlag();
	static shared_ptr<bool> getDrawAnchorFlag();

protected:
	static shared_ptr<bool> drawWireframe;
	static shared_ptr<bool> drawAnchor;

	unsigned int programId;
	BuffersAddresses addresses;
	Uniforms uniforms;

	void initVao();
	void initVbos(const BufferValues bufferValues);
	void initUniformReferences();

	void updateUniformValues(const Transform modelTransform, const Transform meshTransform, const Material material, const optional<shared_ptr<Texture>> texture);
	void passUniforms();
	void bindTexture(const Texture texture) const;
	// Unbinds a texture from the shader. Used when the mesh has no texture.
	void bindNoTexture() const;
	void draw(const BufferValues values) const;
	void checkGLErrors();
};
