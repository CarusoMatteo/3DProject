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

private:
	// Instances of singletons for every shader
	static optional<shared_ptr<ForwardRenderer>> unlitShader;
	static optional<shared_ptr<ForwardRenderer>> phongShader;
	static optional<shared_ptr<ForwardRenderer>> blinnPhongShader;
	static optional<shared_ptr<ForwardRenderer>> reflectionShader;
	static optional<shared_ptr<ForwardRenderer>> cubeMapShader;
	static optional<shared_ptr<ForwardRenderer>> interpolativeShader;

	static optional<shared_ptr<GeometryRenderer>> geometryShader;
};
