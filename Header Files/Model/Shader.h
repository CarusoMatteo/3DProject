#pragma once

#include "Buffers.h"
#include "Uniforms.h"
#include <memory>
#include <string>
#include <vector>

using namespace std;
using namespace glm;

struct Material;
struct ShaderFiles;
struct Transform;

enum ShaderType
{
	UNLIT,
	PHONG,
	BLINN_PHONG,
	REFLECTION
};

class Shader
{
public:
	~Shader();

	void setBufferValues(const BufferValues bufferValues);
	void render(const Transform modelTransform, const Transform meshTransform, const BufferValues values, const Material material);

	static shared_ptr<bool> getDrawWireframeFlag();
	static shared_ptr<bool> getDrawAnchorFlag();
	ShaderType getShaderType() const;

protected:
	Shader(const ShaderFiles files, const ShaderType shaderType);

	static shared_ptr<bool> drawWireframe;
	static shared_ptr<bool> drawAnchor;

	unsigned int programId;
	BuffersAddresses addresses;
	Uniforms uniforms;
	ShaderType shaderType;

	void initVao();
	void initVbos(const BufferValues bufferValues);
	void initUniformReferences();

	void updateUniformValues(const Transform modelTransform, const Transform meshTransform, const Material material);
	void passUniforms();
	void draw(const BufferValues values) const;
	void checkGLErrors();
};

class UnlitShader : public Shader
{
public:
	UnlitShader();
};

class PhongShader : public Shader
{
public:
	PhongShader();
};

class BlinnPhongShader : public Shader
{
public:
	BlinnPhongShader();
};

class ReflectionShader : public Shader
{
public:
	ReflectionShader();
};

class ShaderFactory
{
public:
	static shared_ptr<Shader> createUnlitShader();
	static shared_ptr<Shader> createPhongShader();
	static shared_ptr<Shader> createBlinnPhongShader();
	static shared_ptr<Shader> createReflectionShader();

	static const vector<string> shaderNames;
};
