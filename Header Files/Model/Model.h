#pragma once

#include "Transform.h"
#include <memory>
#include <vector>

using namespace std;

class Mesh;

class Model
{
public:
	Model(const Transform transform, const vector<shared_ptr<Mesh>> meshes);
	~Model() = default;

	void render() const;

	Transform getTransform() const;
	void setTransform(const Transform transform);

private:
	vector<shared_ptr<Mesh>> meshes;
	Transform transform;
};
