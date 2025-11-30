#pragma once

#include <memory>
#include <vector>

using namespace std;

class Mesh;

class Model
{
public:
	Model(const vector<shared_ptr<Mesh>> meshes);
	~Model() = default;

private:
	vector<shared_ptr<Mesh>> meshes;
};
