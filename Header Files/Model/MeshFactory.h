#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>

using namespace glm;
using namespace std;

class Mesh;
struct ShaderFiles;
struct Transform;

class MeshFactory
{
public:
	static shared_ptr<Mesh> plane(const string name, const fvec2 size, const ShaderFiles files, Transform transform);
	static shared_ptr<Mesh> cube(const string name, const float length, const ShaderFiles files, Transform transform);
	static shared_ptr<Mesh> sphere(const string name, const fvec3 radius, const ShaderFiles files, Transform transform);

private:
	MeshFactory() = default;
	~MeshFactory() = default;
};
