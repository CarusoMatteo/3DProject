#pragma once

using namespace std;

class Model;
struct ShaderFiles;

class ModelFactory
{
public:
	static shared_ptr<Model> cube(float length, ShaderFiles files);

private:
	ModelFactory() = default;
	~ModelFactory() = default;
};
