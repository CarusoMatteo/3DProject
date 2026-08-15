#include "../../Header Files/Renderers/ShaderBuilder.h"
#include "../../Header Files/Renderers/ShaderFiles.h"
#include <cstdio>
#include <glad/glad.h>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

unsigned int ShaderBuilder::buildShader(ShaderFiles files)
{
	int success;
	char infoLog[512];

	// Create shader executables
	// Read the Vertex Shader code
	const char *vertexShader = ShaderBuilder::readShaderSource(files.vertex);
	if (!vertexShader)
	{
		cerr << "Failed to load vertex shader source code from file: " << files.vertex << endl;
		throw runtime_error("Failed to load vertex shader source code.");
	}
	else if (ShaderBuilder::shouldPrintLogs)
	{
		cout << "Vertex shader \'" << files.vertex << "\' loaded successfully " << endl;
	}

	// Generate an identifier for the vertex shader
	const unsigned int vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	// Associate the vertex shader code with the identifier
	glShaderSource(vertexShaderId, 1, (const char **)&vertexShader, NULL);
	// Compile the Vertex Shader
	glCompileShader(vertexShaderId);

	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
		cerr << "Error: Vertex shader compilation failed.\n"
			 << infoLog << endl;
		throw runtime_error("Vertex shader compilation failed.");
	}

	// Read the Fragment Shader code
	const GLchar *fragmentShader = ShaderBuilder::readShaderSource(files.fragment);
	if (!fragmentShader)
	{
		cerr << "Failed to load fragment shader source code from file: " << files.fragment << endl;
		throw runtime_error("Failed to load fragment shader source code.");
	}
	else if (ShaderBuilder::shouldPrintLogs)
	{
		cout << "Fragment shader \'" << files.fragment << "\' loaded successfully " << endl;
	}

	// Generate an identifier for the FRAGMENT shader
	const unsigned int fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, (const char **)&fragmentShader, NULL);
	// Compile the FRAGMENT Shader
	glCompileShader(fragmentShaderId);

	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog);
		cerr << "Error: Fragment shader compilation failed.\n"
			 << infoLog << endl;
		throw runtime_error("Fragment shader compilation failed.");
	}

	unsigned int geometryShaderId = 0;
	// Read the geometry shader code if provided
	if (files.geometry.has_value())
	{
		// Read the Geometry Shader code
		const GLchar *geometryShader = ShaderBuilder::readShaderSource(files.geometry.value());
		if (!geometryShader)
		{
			cerr << "Failed to load geometry shader source code from file: " << files.geometry.value() << endl;
			throw runtime_error("Failed to load geometry shader source code.");
		}
		else if (ShaderBuilder::shouldPrintLogs)
		{
			cout << "Geometry shader \'" << files.geometry.value() << "\' loaded successfully " << endl;
		}

		// Generate an identifier for the GEOMETRY shader
		geometryShaderId = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShaderId, 1, (const char **)&geometryShader, NULL);
		// Compile the GEOMETRY Shader
		glCompileShader(geometryShaderId);

		glGetShaderiv(geometryShaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(geometryShaderId, 512, NULL, infoLog);
			cerr << "Error: Geometry shader compilation failed.\n"
				 << infoLog << endl;
			throw runtime_error("Geometry shader compilation failed.");
		}
	}

	// Create an identifier for a program and attach the two compiled shaders to it
	const unsigned int programId = glCreateProgram();

	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	if (files.geometry.has_value())
	{
		glAttachShader(programId, geometryShaderId);
	}
	glLinkProgram(programId);

	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programId, 512, NULL, infoLog);
		std::cout << "Error: Linking program failed.\n"
				  << infoLog << std::endl;
		throw runtime_error("Linking program failed.");
	}

	return programId;
}

#pragma warning(disable : 6386)
#pragma warning(disable : 4996)
char *ShaderBuilder::readShaderSource(const string file)
{
	FILE *fp = fopen(file.c_str(), "rb");

	if (fp == NULL)
	{
		cerr << "Failed to open shader file: " << file << endl;
		throw runtime_error("Failed to open shader file: " + file);
	}

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char *buf = new char[size + 1];
	fread(buf, 1, size, fp);

	buf[size] = '\0';
	fclose(fp);

	return buf;
}
