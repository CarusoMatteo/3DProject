#pragma once

#include "Texture.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>

using namespace glm;
using namespace std;

class CubeMapMesh;
class Mesh;
class Shader;
struct Transform;

class MeshFactory
{
public:
	static shared_ptr<Mesh> plane(const string name, const fvec2 size, const shared_ptr<Shader> shader, Transform transform, const shared_ptr<Texture> texture);
	static shared_ptr<Mesh> cube(const string name, const float length, const shared_ptr<Shader> shader, Transform transform, const shared_ptr<Texture> texture);
	static shared_ptr<Mesh> sphere(const string name, const fvec3 radius, const shared_ptr<Shader> shader, Transform transform, const shared_ptr<Texture> texture);

private:
	MeshFactory() = default;
	~MeshFactory() = default;
};
