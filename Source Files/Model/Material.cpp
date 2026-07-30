#include "../../Header Files/Model/Material.h"
#include <string>
#include <vector>

using namespace glm;
using namespace std;

const vector<string> MaterialsFactory::names = {
	"Red Plastic",
	"Brass",
	"Emerald",
	"Snow White",
	"Yellow",
	"Pink",
	"Brown",
	"Custom Material"};

const Material MaterialsFactory::redPlastic = {
	"Red Plastic",
	fvec3(0.1, 0.0, 0.0),
	fvec3(0.6, 0.1, 0.1),
	fvec3(0.7, 0.6, 0.6),
	150.0f};
const Material MaterialsFactory::brass = {
	"Brass",
	fvec3(0.5, 0.06, 0.015),
	fvec3(0.78, 0.57, 0.11),
	fvec3(0.99, 0.91, 0.81),
	27.8f};
const Material MaterialsFactory::emerald = {
	"Emerald",
	fvec3(0.0215, 0.04745, 0.0215),
	fvec3(0.07568, 0.61424, 0.07568),
	fvec3(0.633, 0.727811, 0.633),
	78.8f};
const Material MaterialsFactory::snowWhite = {
	"Snow White",
	fvec3(0.2, 0.2, 0.2),
	fvec3(0.95, 0.96, 0.98),
	fvec3(0.8, 0.8, 0.8),
	1.78125f};
const Material MaterialsFactory::yellow = {
	"Yellow",
	fvec3(0.8, 0.8, 0.0),
	fvec3(1.0, 1.0, 0.6),
	fvec3(0.9, 0.9, 0.04),
	1.78125f};
const Material MaterialsFactory::pink = {
	"Pink",
	fvec3(0.05f, 0.0f, 0.0f),
	fvec3(0.5f, 0.4f, 0.4f),
	fvec3(0.7f, 0.04f, 0.04f),
	1.78125f};
const Material MaterialsFactory::brown = {
	"Brown",
	fvec3(0.19125f, 0.0735f, 0.0225f),
	fvec3(0.7038f, 0.27048f, 0.0828f),
	fvec3(0.256777f, 0.137622f, 0.086014f),
	12.8f};

Material MaterialsFactory::makeMaterial(MaterialType type)
{
	switch (type)
	{
	case RED_PLASTIC:
		return redPlastic;
	case BRASS:
		return brass;
	case EMERALD:
		return emerald;
	case SNOW_WHITE:
		return snowWhite;
	case YELLOW:
		return yellow;
	case PINK:
		return pink;
	case BROWN:
		return brown;
	default:
		return Material();
	}
}
