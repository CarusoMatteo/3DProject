#pragma once

#include "../Renderers/Renderer.h"
#include "../Texture/Texture.h"
#include <functional>
#include <memory>
#include <optional>
#include <string>

using namespace std;

class Model;
struct Transform;

Model loadModel(const string modelName, const function<shared_ptr<Renderer>()> rendererFactoryFunction, const Transform modelTransform, const optional<shared_ptr<Texture>> texture);
