#pragma once

#include <string>

using namespace std;

struct ShaderFiles;

class ShaderBuilder
{
public:
	static unsigned int buildShader(ShaderFiles files);

private:
	static const bool shouldPrintLogs = false;
	static char *readShaderSource(const string file);

	ShaderBuilder() = default;
	~ShaderBuilder() = default;
};
