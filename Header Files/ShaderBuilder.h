#pragma once

#include <string>
#include <glad/glad.h>

using namespace std;

/**
 * @brief A utility class for building OpenGL shaders from source files.
 */
class ShaderBuilder
{
public:
	/**
	 * @brief Builds a shader program from vertex and fragment shader source files.
	 *
	 * @param vertexfilename The path to the vertex shader source file.
	 * @param fragmentfilename The path to the fragment shader source file.
	 * @return GLuint The ID of the created shader program.
	 */
	static GLuint buildShader(const string vertexfilename, const string fragmentfilename);

private:
	ShaderBuilder() = default;
	~ShaderBuilder() = default;

	/// @brief Whether to print success logs during shader compilation and linking.
	static bool shouldPrintLogs;
	static char *readShaderSource(const string shaderFile);
};
