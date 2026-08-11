#include "../../Header Files/Model/ModelLoader.h"
#include "../../Header Files/Model/Material.h"
#include "../../Header Files/Model/Mesh.h"
#include "../../Header Files/Model/Model.h"
#include "../../Header Files/Model/Transform.h"
#include "../../Header Files/Renderers/BuffersValues.h"
#include "../../Header Files/Renderers/ShaderFactory.h"
#include <assimp/Importer.hpp>	// C++ importer interface
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>		// Output data structure
#include <cstdio>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace std;
using namespace glm;

Material loadMaterial(aiMaterial *material, aiColor3D &color, const std::string &path, std::string &meshName, float &value);
vector<fvec3> loadVertices(const aiMesh *mesh);
pair<vector<fvec2>, vector<fvec4>> loadTextureCoordinates(const aiMesh *mesh);
vector<fvec3> loadNormals(const aiMesh *mesh);
vector<unsigned int> loadIndices(const aiMesh *mesh);

// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
Model loadModel(const Transform transform, const string path)
{
	Assimp::Importer importer;
	vector<shared_ptr<Mesh>> meshes;

	const aiScene *scene = importer.ReadFile(path, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene)
	{
		cout << "Error in loadModel:" << importer.GetErrorString() << endl;
		throw runtime_error("Error in loadModel");
	}
	const aiMesh *mesh;

	// Fill vertices positions
	// Number of meshes that make the model
	unsigned int num_meshes = scene->mNumMeshes;
	// Resize meshes to the number of meshes that make up the object
	meshes.resize(num_meshes);

	// For each mesh of the object
	for (unsigned int nm = 0; nm < num_meshes; nm++)
	{
		mesh = scene->mMeshes[nm];
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

		aiColor3D color;
		float value;
		string meshName = mesh->mName.C_Str();

		Material mat = loadMaterial(material, color, path, meshName, value);
		vector<fvec3> vertices = loadVertices(mesh);
		vector<fvec3> normals = loadNormals(mesh);
		auto [textureCoordinates, colors] = loadTextureCoordinates(mesh);
		vector<unsigned int> indices = loadIndices(mesh);

		ForwardBufferValues forwardBufferValues = {vertices, colors, normals, indices, textureCoordinates};
		GeometryBufferValues geometryBufferValues = {};
		auto shader = ShaderFactory::geometry();
		shader->setBufferValues(geometryBufferValues, forwardBufferValues);

		meshes.push_back(make_shared<Mesh>(meshName, shader, Transform(), mat, nullopt));
	}

	return Model(transform, meshes);
}

Material loadMaterial(aiMaterial *material, aiColor3D &color, const std::string &path, std::string &meshName, float &value)
{
	// Read mtl file vertex data
	fvec3 ambient, diffuse, specular;
	float shininess;

	if (aiReturn_SUCCESS == material->Get(AI_MATKEY_COLOR_AMBIENT, color))
	{
		ambient = fvec3(color.r, color.g, color.b);
	}
	else
	{
		cout << "Error in loading ambient for mesh " << path << " -> " << meshName << ", using default. \n"
			 << endl;
		ambient = fvec3(0.2, 0.2, 0.2);
	}

	if (aiReturn_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, color))
	{
		diffuse = fvec3(color.r, color.g, color.b);
	}
	else
	{
		cout << "Error in loading diffuse for mesh " << path << " -> " << meshName << ", using default. \n"
			 << endl;
		diffuse = fvec3(1.0, 0.2, 0.1);
	}

	if (aiReturn_SUCCESS == material->Get(AI_MATKEY_COLOR_SPECULAR, color))
	{
		specular = fvec3(color.r, color.g, color.b);
	}
	else
	{
		cout << "Error in loading specular for mesh " << path << " -> " << meshName << ", using default. \n"
			 << endl;
		specular = fvec3(0.5, 0.5, 0.5);
	}
	if (aiReturn_SUCCESS == material->Get(AI_MATKEY_SHININESS_STRENGTH, value))
	{
		shininess = value;
	}
	else
	{
		cout << "Error in loading shininess for mesh " << path << " -> " << meshName << ", using default. \n"
			 << endl;
		shininess = 50.0f;
	}
	return {meshName, ambient, diffuse, specular, shininess};
}

vector<fvec3> loadVertices(const aiMesh *mesh)
{
	vector<glm::fvec3> vertices;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		aiVector3D pos = mesh->mVertices[i];
		vertices.push_back(fvec3(pos.x, pos.y, pos.z));
	}
	return vertices;
}

// Returns texture coordinates and fallback colors.
pair<vector<fvec2>, vector<fvec4>> loadTextureCoordinates(const aiMesh *mesh)
{
	vector<fvec2> textureCoordinates;
	vector<fvec4> colors;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		textureCoordinates.push_back(fvec2(0.0, 0.0));
		// Default color assigned to each vertex, used if the mesh has no texture.
		colors.push_back(fvec4(1.0, 0.0, 1.0, 1.0));
	}
	return make_pair(textureCoordinates, colors);
}

vector<fvec3> loadNormals(const aiMesh *mesh)
{
	// Fill vertices normals
	vector<fvec3> normals;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		aiVector3D n = mesh->mNormals[i];
		normals.push_back(fvec3(n.x, n.y, n.z));
	}
	return normals;
}

vector<unsigned int> loadIndices(const aiMesh *mesh)
{
	vector<unsigned int> indices;
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		// Assume the model has only triangles.
		indices.push_back(mesh->mFaces[i].mIndices[0]);
		indices.push_back(mesh->mFaces[i].mIndices[1]);
		indices.push_back(mesh->mFaces[i].mIndices[2]);
	}
	return indices;
}