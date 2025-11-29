#pragma once

#include <vector>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

class ShapeMaker
{
public:
private:
	ShapeMaker() = default;
	~ShapeMaker() = default;

	static void addBoundingBoxVertices(const vector<fvec3> *vertices, const vector<fvec4> *colors);
};