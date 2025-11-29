#pragma once

#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

class BoundingBox
{
public:
	BoundingBox() = default;
	~BoundingBox() = default;

private:
	vector<fvec3> vertices;
};
