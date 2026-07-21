#pragma once

#include "../Model/CubeMapMesh.h"
#include "../Model/CubeMapModel.h"
#include "IVisibleGameObject.h"
#include <memory>
#include <string>
#include <vector>

using namespace std;

class Mesh;
struct Transform;

class CubeMap : public IVisibleGameObject
{
public:
	// Singleton access.
	static CubeMap *I();

	~CubeMap() = default;

	void update(const float deltaTime) override;
	void render() override;

	Transform getTransform() const override;
	void setTransform(const Transform transform) override;
	vector<shared_ptr<Mesh>> getMeshes() const override;
	shared_ptr<CubeMapMesh> getMesh() const;
	string getName() const override;

	static vector<string> getFacesPaths(string path);

private:
	static unique_ptr<CubeMap> instance;

	shared_ptr<CubeMapModel> model;

	CubeMap();
};
