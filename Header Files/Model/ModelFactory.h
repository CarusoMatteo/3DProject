#pragma once

#include <functional>
#include <glm/glm.hpp>
#include <memory>
#include <optional>
#include <string>

using namespace glm;
using namespace std;

class CubeMapModel;
class Model;
class Renderer;
struct Texture;
struct Transform;

class ModelFactory
{
public:
	static shared_ptr<Model> plane(const string name, const fvec2 size, const shared_ptr<Renderer> shader, const Transform modelTransform, const optional<shared_ptr<Texture>> texture);
	static shared_ptr<Model> cube(const string name, const float length, const shared_ptr<Renderer> shader, const Transform modelTransform, const optional<shared_ptr<Texture>> texture);
	static shared_ptr<Model> sphere(const string name, const fvec3 radius, const shared_ptr<Renderer> shader, const Transform modelTransform, const optional<shared_ptr<Texture>> texture);
	static shared_ptr<Model> customModel(const string modelPath, const function<shared_ptr<Renderer>()> rendererFactoryFunction, const Transform modelTransform, const optional<shared_ptr<Texture>> texture);

private:
	ModelFactory() = default;
	~ModelFactory() = default;
};
