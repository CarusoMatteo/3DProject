#pragma once

#include "Buffers.h"
#include "Uniforms.h"

using namespace std;
using namespace glm;

struct ShaderFiles;

class Shader
{
public:
	Shader(const ShaderFiles files, const BufferValues bufferValues);
	~Shader() = default;

	void render();

private:
	unsigned int programId;
	BuffersAddresses buffers;
	Uniforms uniforms;

	void initVao();
	void initVbos(const BufferValues bufferValues);
	void initUniformReferences();
};
