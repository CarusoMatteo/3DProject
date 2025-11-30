#include "../../Header Files/Model/Model.h"
#include <memory>
#include <vector>

Model::Model(vector<shared_ptr<Mesh>> meshes)
{
	this->meshes = meshes;
}
