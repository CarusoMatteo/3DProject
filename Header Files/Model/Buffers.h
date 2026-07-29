#pragma once

#include <glm/glm.hpp>
#include <vector>

using namespace std;
using namespace glm;

struct BuffersAddresses
{
	unsigned int vao = -1;

	// VBOs:

	unsigned int vertices = -1;
	unsigned int colors = -1;
	unsigned int normals = -1;
	unsigned int indices = -1;
	unsigned int textureCoordinates = -1;
};

struct BufferValues
{
	vector<fvec3> vertices;
	vector<fvec4> colors;
	vector<fvec3> normals;
	vector<unsigned int> indices;
	vector<fvec2> textureCoordinates;
	// fvec4 tintColor;
};