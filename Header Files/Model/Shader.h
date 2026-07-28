#pragma once

#include "Buffers.h"
#include "Texture.h"
#include "Uniforms.h"
#include <memory>
#include <optional>
#include <string>

using namespace std;
using namespace glm;

struct Material;
struct ShaderFiles;
struct Transform;

class Shader
{
public:
	Shader(const ShaderFiles files);
	~Shader();

	void setBufferValues(const BufferValues bufferValues);
	void render(const Transform modelTransform, const Transform meshTransform, const BufferValues values, const Material material, const optional<shared_ptr<Texture>> texture);

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

	void updateUniformValues(const Transform modelTransform, const Transform meshTransform, const Material material);
	void passUniforms();
	void bindTexture(const Texture texture) const;
	// Unbinds a texture from the shader. Used when the mesh has no texture.
	void bindNoTexture() const;
	void draw(const BufferValues values) const;
	void checkGLErrors();
};

class ShaderFactory
{
public:
	static shared_ptr<Shader> unlit();
	static shared_ptr<Shader> phong();
	static shared_ptr<Shader> blinnPhong();
	static shared_ptr<Shader> reflection();
	static shared_ptr<Shader> cubeMap();

private:
	// Instances of singletons for every shader
	static optional<shared_ptr<Shader>> unlitShader;
	static optional<shared_ptr<Shader>> phongShader;
	static optional<shared_ptr<Shader>> blinnPhongShader;
	static optional<shared_ptr<Shader>> reflectionShader;
	static optional<shared_ptr<Shader>> cubeMapShader;

	// Paths to the shader files
	static const string SHADER_PATH;
	static const ShaderFiles UNLIT_PATH;
	static const ShaderFiles PHONG_PATH;
	static const ShaderFiles BLINN_PHONG_PATH;
	static const ShaderFiles REFLECTION_PATH;
	static const ShaderFiles CUBE_MAP_PATH;
};
