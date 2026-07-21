#pragma once

#include "CubeMapMesh.h"
#include "Transform.h"
#include <memory>

using namespace std;

class CubeMapModel
{
public:
	CubeMapModel(const Transform transform, const shared_ptr<CubeMapMesh> mesh);
	void render() const;

	Transform getTransform() const;
	shared_ptr<CubeMapMesh> getMesh() const;
	void setTransform(const Transform transform);

private:
	shared_ptr<CubeMapMesh> mesh;
	Transform transform;
};
