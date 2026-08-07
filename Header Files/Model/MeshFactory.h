#pragma once

#include "../Texture/Texture.h"
#include <glm/glm.hpp>
#include <memory>
#include <optional>
#include <string>

using namespace glm;
using namespace std;

class CubeMapMesh;
class Mesh;
class ForwardRenderer;
class Renderer;
struct Transform;

class MeshFactory
{
public:
	static shared_ptr<Mesh> plane(const string name, const fvec2 size, const shared_ptr<Renderer> shader, Transform transform, const optional<shared_ptr<Texture>> texture);
	static shared_ptr<Mesh> cube(const string name, const float length, const shared_ptr<Renderer> shader, Transform transform, const optional<shared_ptr<Texture>> texture);
	static shared_ptr<Mesh> sphere(const string name, const fvec3 radius, const shared_ptr<Renderer> shader, Transform transform, const optional<shared_ptr<Texture>> texture);
	static shared_ptr<Mesh> screenQuad(shared_ptr<ForwardRenderer> shader);

private:
	MeshFactory() = default;
	~MeshFactory() = default;
};
