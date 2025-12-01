#pragma once

#include <glm/glm.hpp>
#include <string>

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

struct Materials
{
	Material redPlastic = {
		"Red Plastic",
		fvec3(0.1, 0.0, 0.0),
		fvec3(0.6, 0.1, 0.1),
		fvec3(0.7, 0.6, 0.6),
		150.0f};
	Material brass = {
		"Brass",
		fvec3(0.5, 0.06, 0.015),
		fvec3(0.78, 0.57, 0.11),
		fvec3(0.99, 0.91, 0.81),
		27.8f};
	Material emerald = {
		"Emerald",
		fvec3(0.0215, 0.04745, 0.0215),
		fvec3(0.07568, 0.61424, 0.07568),
		fvec3(0.633, 0.727811, 0.633),
		78.8f};
	Material snowWhite = {
		"Snow White",
		fvec3(0.2, 0.2, 0.2),
		fvec3(0.95, 0.96, 0.98),
		fvec3(0.8, 0.8, 0.8),
		1.78125f};
	Material yellow = {
		"Yellow",
		fvec3(0.8, 0.8, 0.0),
		fvec3(1.0, 1.0, 0.6),
		fvec3(0.9, 0.9, 0.04),
		1.78125f};
	Material pink = {
		"Pink",
		fvec3(0.05f, 0.0f, 0.0f),
		fvec3(0.5f, 0.4f, 0.4f),
		fvec3(0.7f, 0.04f, 0.04f),
		1.78125f};
	Material brown = {
		"Brown",
		fvec3(0.19125f, 0.0735f, 0.0225f),
		fvec3(0.7038f, 0.27048f, 0.0828f),
		fvec3(0.256777f, 0.137622f, 0.086014f),
		12.8f};
};
