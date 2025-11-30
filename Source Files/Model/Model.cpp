#include "../../Header Files/Model/Model.h"
#include <memory>
#include <vector>

Model::Model(const vector<shared_ptr<Mesh>> meshes)
{
	this->meshes = meshes;
}
