#pragma once

#include <memory>
#include <optional>

using namespace std;
using namespace glm;

struct BufferValues;
struct Material;
struct Transform;
struct Texture;

class IShader
{
public:
	virtual void setBufferValues(const BufferValues bufferValues) = 0;
	virtual void render(const Transform modelTransform, const Transform meshTransform, const BufferValues values, const Material material, const optional<shared_ptr<Texture>> texture) = 0;
};
