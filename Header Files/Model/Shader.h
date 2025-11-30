#pragma once

#include <string>

using namespace std;

struct ShaderFiles;

class Shader
{
public:
	Shader(ShaderFiles files);
	~Shader() = default;

	void render();

private:
	unsigned int programId;
	unsigned int vaoAddress;
	unsigned int verticesVboAddress;
	unsigned int colorsVboAddress;
	unsigned int normalsVboAddress;
	unsigned int indicesVboAddress;
	unsigned int textureCoordsVboAddress;

	void buildShader(const string vertexShaderName, const string fragmentShaderName);
	void initVao();
	void initVbos();
	void initUniformReferences();
};
