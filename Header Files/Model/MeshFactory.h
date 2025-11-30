#pragma once

#include <memory>

using namespace std;

class Mesh;
struct ShaderFiles;
struct Transform;

class MeshFactory
{
public:
	static shared_ptr<Mesh> cube(const float length, const ShaderFiles files, Transform transform);

private:
	MeshFactory() = default;
	~MeshFactory() = default;
};
