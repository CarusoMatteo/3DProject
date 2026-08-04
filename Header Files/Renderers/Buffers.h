#pragma once

#include <glm/glm.hpp>
#include <vector>

using namespace std;
using namespace glm;

typedef void *unknown;

struct ForwardBufferAddresses
{
	unsigned int vao = -1;

	// VBOs:
	unsigned int vertices = -1;
	unsigned int colors = -1;
	unsigned int normals = -1;
	unsigned int indices = -1;
	unsigned int textureCoordinates = -1;
};

// Buffer that contain the geometric information for the geometry pass
struct DeferredBuffersAddresses
{
	unsigned int gBuffer = -1;

	unsigned int positions = -1;
	unsigned int albedosSpeculars = -1;
	unsigned int normals = -1;
	unsigned int depths = -1;
};

struct ForwardBufferValues
{
	vector<fvec3> vertices;
	vector<fvec4> colors;
	vector<fvec3> normals;
	vector<unsigned int> indices;
	vector<fvec2> textureCoordinates;
};

struct DeferredBufferValues
{
	// 3D World spaceposition to interpolate the fragment position.
	vector<fvec3> positions;
	// RGB diffuse color vector and the specular intensity in the alpha channel.
	vector<fvec4> albedosSpeculars;
	// 3D normal vector to determine the slope of the surface.
	vector<fvec3> normals;
	vector<float> depths;
};
