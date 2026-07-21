#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>

using namespace glm;
using namespace std;

class CubeMapModel;
class Model;
class Shader;
struct Transform;

class ModelFactory
{
public:
	static shared_ptr<Model> plane(const string name, const fvec2 size, const shared_ptr<Shader> shader, Transform modelTransform);
	static shared_ptr<Model> cube(const string name, const float length, const shared_ptr<Shader> shader, Transform modelTransform);
	static shared_ptr<Model> sphere(const string name, const fvec3 radius, const shared_ptr<Shader> shader, Transform modelTransform);
	static shared_ptr<CubeMapModel> cubeMap(const string name, const float length, const shared_ptr<Shader> shader, Transform modelTransform);

private:
	ModelFactory() = default;
	~ModelFactory() = default;
};
