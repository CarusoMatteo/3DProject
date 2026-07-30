#include "../Header Files/ShaderBuilder.h"
#include "../Header Files/Model/ShaderFiles.h"
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
		throw new runtime_error("Failed to load vertex shader source code.");
	}
	else if (ShaderBuilder::shouldPrintLogs)
	{
		std::cout << "Vertex shader \'" << files.vertex << "\' loaded succesfully " << std::endl;
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
		std::cout << "Error: Vertex shader compilation failed.\n"
				  << infoLog << std::endl;
		throw new std::runtime_error("Vertex shader compilation failed.");
	}

	// Read the Fragment Shader code
	const GLchar *fragmentShader = ShaderBuilder::readShaderSource(files.fragment);
	if (!fragmentShader)
	{
		throw new std::runtime_error("Failed to load fragment shader source code.");
	}
	else if (ShaderBuilder::shouldPrintLogs)
	{
		std::cout << "Fragment shader \'" << files.fragment << "\' loaded succesfully " << std::endl;
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
		std::cout << "Error: Fragment shader compilation failed.\n"
				  << infoLog << std::endl;
		throw new std::runtime_error("Fragment shader compilation failed.");
	}

	// Create an identifier for a program and attach the two compiled shaders to it
	const unsigned int programId = glCreateProgram();

	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);

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
		throw new std::runtime_error("Failed to open shader file: " + file);
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
