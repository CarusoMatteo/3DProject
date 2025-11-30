#pragma once

#include "Buffers.h"
#include "Uniforms.h"

using namespace std;
using namespace glm;

struct ShaderFiles;
struct Transform;

class Shader
{
public:
	Shader(const ShaderFiles files, const BufferValues bufferValues);
	~Shader();

	void render(const Transform model, const BufferValues values);

private:
	unsigned int programId;
	BuffersAddresses addresses;
	Uniforms uniforms;

	void initVao();
	void initVbos(const BufferValues bufferValues);
	void initUniformReferences();

	void updateUniformValues(const Transform model);
	void passUniforms();
	void draw(const BufferValues values) const;
	void checkGLErrors();
};
