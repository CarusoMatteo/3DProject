#pragma once

#include <memory>

using namespace std;

class Model;
struct ShaderFiles;

class ModelFactory
{
public:
	static shared_ptr<Model> cube(const float length, const ShaderFiles files);

private:
	ModelFactory() = default;
	~ModelFactory() = default;
};
