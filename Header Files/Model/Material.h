#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

using namespace glm;
using namespace std;

struct Material
{
	string name = "";
	fvec3 ambient = fvec3(0);
	fvec3 diffuse = fvec3(0);
	fvec3 specular = fvec3(0);
	float shininess = 0.0f;
};

enum MaterialType
{
	RED_PLASTIC,
	BRASS,
	EMERALD,
	SNOW_WHITE,
	YELLOW,
	PINK,
	BROWN,
	NO_MATERIAL,
	CUSTOM_MATERIAL,
	NUMBER
};

class MaterialsFactory
{
public:
	static Material makeMaterial(MaterialType type);

private:
	static const Material redPlastic;
	static const Material brass;
	static const Material emerald;
	static const Material snowWhite;
	static const Material yellow;
	static const Material pink;
	static const Material brown;
};
