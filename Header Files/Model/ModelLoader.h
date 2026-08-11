#pragma once

#include <string>

using namespace std;

class Model;
struct Transform;

Model loadModel(const Transform transform, const string path);
