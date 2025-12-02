#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>

using namespace glm;
using namespace std;

class Model;
struct ShaderFiles;
struct Transform;

class ModelFactory
{
public:
	static shared_ptr<Model> plane(const string name, const fvec2 size, const ShaderFiles files, Transform modelTransform);
	static shared_ptr<Model> cube(const string name, const float length, const ShaderFiles files, Transform modelTransform);
	static shared_ptr<Model> sphere(const string name, const fvec3 radius, const ShaderFiles files, Transform modelTransform);

private:
	ModelFactory() = default;
	~ModelFactory() = default;
};
