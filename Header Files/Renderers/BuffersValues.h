#pragma once

#include <glm/glm.hpp>
#include <vector>

using namespace std;
using namespace glm;

struct ForwardBufferValues
{
	vector<fvec3> vertices;
	vector<fvec4> colors;
	vector<fvec3> normals;
	vector<unsigned int> indices;
	vector<fvec2> textureCoordinates;
};

struct GeometryBufferValues
{
	// 3D World spaceposition to interpolate the fragment position.
	vector<fvec3> positions;
	// RGB diffuse color vector and the specular intensity in the alpha channel.
	vector<fvec4> albedosSpecular;
	// 3D normal vector to determine the slope of the surface.
	vector<fvec3> normals;
	vector<float> depths;
};

struct SingleBufferValues
{
	vector<float> values;
};