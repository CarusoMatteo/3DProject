#pragma once

#include "Buffers.h"
#include "Uniforms.h"
#include <memory>

using namespace std;
using namespace glm;

struct ShaderFiles;
struct Transform;

class Shader
{
public:
	Shader(const ShaderFiles files, const BufferValues bufferValues);
	~Shader();

	void render(const Transform modelTransform, const Transform meshTransform, const BufferValues values);

	static shared_ptr<bool> getDrawWireframeFlag();
	static shared_ptr<bool> getDrawAnchorFlag();

private:
	static shared_ptr<bool> drawWireframe;
	static shared_ptr<bool> drawAnchor;

	unsigned int programId;
	BuffersAddresses addresses;
	Uniforms uniforms;

	void initVao();
	void initVbos(const BufferValues bufferValues);
	void initUniformReferences();

	void updateUniformValues(const Transform modelTransform, const Transform meshTransform);
	void passUniforms();
	void draw(const BufferValues values) const;
	void checkGLErrors();
};
