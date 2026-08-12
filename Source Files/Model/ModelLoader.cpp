#include "../../Header Files/Model/ModelLoader.h"
#include "../../Header Files/Model/Material.h"
#include "../../Header Files/Model/Mesh.h"
#include "../../Header Files/Model/Model.h"
#include "../../Header Files/Model/Transform.h"
#include "../../Header Files/Renderers/BuffersValues.h"
#include "../../Header Files/Renderers/ForwardRenderer.h"
#include "../../Header Files/Renderers/GeometryRenderer.h"
#include "../../Header Files/Renderers/Renderer.h"
#include "../../Header Files/Texture/Texture.h"
#include "../../Header Files/Texture/TextureLoader.h"
#include <assimp/Importer.hpp>	// C++ importer interface
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>		// Output data structure
#include <functional>
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

static bool isForwardRenderer(const shared_ptr<Renderer> shader)
{
	// Check if the shader is a ForwardRenderer by dynamic casting it.
	return dynamic_pointer_cast<ForwardRenderer>(shader) != nullptr;
}

static bool isGeometryRenderer(const shared_ptr<Renderer> shader)
{
	// Check if the shader is a DeferredRenderer by dynamic casting it.
	return dynamic_pointer_cast<GeometryRenderer>(shader) != nullptr;
}

// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
Model loadModel(const string modelName, const function<shared_ptr<Renderer>()> rendererFactoryFunction, const Transform modelTransform, const optional<shared_ptr<Texture>> texture)
{
	Assimp::Importer importer;
	vector<shared_ptr<Mesh>> meshes;
	string modelPath = "./assets/models/" + modelName + "/" + modelName + ".obj";

	const aiScene *scene = importer.ReadFile(modelPath, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_FlipUVs);
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

	// For each mesh of the object
	for (unsigned int nm = 0; nm < num_meshes; nm++)
	{
		mesh = scene->mMeshes[nm];
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

		aiColor3D color;
		float value;
		string meshName = mesh->mName.C_Str();

		Material mat = loadMaterial(material, color, modelPath, meshName, value);
		vector<fvec3> vertices = loadVertices(mesh);
		vector<fvec3> normals = loadNormals(mesh);
		auto [textureCoordinates, colors] = loadTextureCoordinates(mesh);
		vector<unsigned int> indices = loadIndices(mesh);

		shared_ptr<Renderer> shader = rendererFactoryFunction();

		const ForwardBufferValues forwardBufferValues = {vertices, colors, normals, indices, textureCoordinates};
		if (isForwardRenderer(shader))
		{
			dynamic_pointer_cast<ForwardRenderer>(shader)->setBufferValues(forwardBufferValues);
		}
		else if (isGeometryRenderer(shader))
		{
			const GeometryBufferValues geometryBufferValues = {
				vertices,					   // positions
				vector<fvec4>(4, fvec4(1.0f)), // albedo + specular
				normals,					   // normals
				vector<float>(4, 0.0f)		   // depths, if required
			};
			dynamic_pointer_cast<GeometryRenderer>(shader)->setBufferValues(geometryBufferValues, forwardBufferValues);
		}
		else
		{
			cerr << "Error: Unknown shader type. Cannot set buffer values." << endl;
			throw runtime_error("Unknown shader type. Cannot set buffer values.");
		}
		meshes.push_back(make_shared<Mesh>(meshName, shader, Transform(), mat, texture));
	}

	return Model(modelTransform, meshes);
}

Material loadMaterial(aiMaterial *material, aiColor3D &color, const std::string &modelPath, std::string &meshName, float &value)
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
		cout << "Error in loading ambient for mesh " << modelPath << " -> " << meshName << ", using default." << endl;
		ambient = fvec3(0.2, 0.2, 0.2);
	}

	if (aiReturn_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, color))
	{
		diffuse = fvec3(color.r, color.g, color.b);
	}
	else
	{
		cout << "Error in loading diffuse for mesh " << modelPath << " -> " << meshName << ", using default." << endl;
		diffuse = fvec3(1.0, 0.2, 0.1);
	}

	if (aiReturn_SUCCESS == material->Get(AI_MATKEY_COLOR_SPECULAR, color))
	{
		specular = fvec3(color.r, color.g, color.b);
	}
	else
	{
		cout << "Error in loading specular for mesh " << modelPath << " -> " << meshName << ", using default." << endl;
		specular = fvec3(0.5, 0.5, 0.5);
	}
	if (aiReturn_SUCCESS == material->Get(AI_MATKEY_SHININESS_STRENGTH, value))
	{
		shininess = value;
	}
	else
	{
		cout << "Error in loading shininess for mesh " << modelPath << " -> " << meshName << ", using default." << endl;
		shininess = 50.0f;
	}
	return {meshName, ambient, diffuse, specular, shininess};
}

vector<fvec3> loadVertices(const aiMesh *mesh)
{
	vector<fvec3> vertices;
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
		// If mesh has texture coordinates:
		if (mesh->mTextureCoords[0])
		{
			// Load them
			aiVector3D texCoord = mesh->mTextureCoords[0][i];
			textureCoordinates.push_back(fvec2(texCoord.x, texCoord.y));
		}
		else
		{
			cout << "Error in loading texture coordinates for mesh " << mesh->mName.C_Str() << ", using default." << endl;
			textureCoordinates.push_back(fvec2(0.0, 0.0));
		}

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
