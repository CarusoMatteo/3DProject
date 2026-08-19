#pragma once

#include "RenderMode.h"
#include <memory>
#include <optional>

using namespace std;

struct ForwardBufferValues;
struct Material;
struct Transform;
struct Texture;

class Renderer
{
public:
	virtual void render(const float currentTime, const Transform modelTransform, const Transform meshTransform, const Material material, const optional<shared_ptr<Texture>> texture) = 0;

	static shared_ptr<bool> getDrawWireframeFlag();
	static shared_ptr<bool> getDrawAnchorFlag();
	static void setRenderMode(const RenderMode mode);

protected:
	static shared_ptr<bool> drawWireframe;
	static shared_ptr<bool> drawAnchor;
	static shared_ptr<RenderMode> renderMode;

	bool bufferIsUsed(const unsigned int address, const size_t size) const;
	void checkGLErrors() const;
};
