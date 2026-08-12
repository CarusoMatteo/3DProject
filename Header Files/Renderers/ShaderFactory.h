#pragma once

#include "ForwardRenderer.h"
#include "GeometryRenderer.h"
#include <memory>
#include <optional>

class ShaderFactory
{
public:
	static shared_ptr<ForwardRenderer> unlit();
	static shared_ptr<ForwardRenderer> phong();
	static shared_ptr<ForwardRenderer> blinnPhong();
	static shared_ptr<ForwardRenderer> reflection();
	static shared_ptr<ForwardRenderer> cubeMap();
	static shared_ptr<ForwardRenderer> interpolative();

	static shared_ptr<GeometryRenderer> geometry();
};
