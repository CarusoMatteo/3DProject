#include "../../Header Files/Model/Shader.h"
#include "../../Header Files/Model/ShaderFiles.h"
#include <string>

Shader::Shader(ShaderFiles files)
{
	this->buildShader(files.vertex, files.fragment);
}

void Shader::render()
{
}

void Shader::buildShader(const string vertexShaderName, const string fragmentShaderName)
{
}

void Shader::initVao()
{
}

void Shader::initVbos()
{
}

void Shader::initUniformReferences()
{
}
