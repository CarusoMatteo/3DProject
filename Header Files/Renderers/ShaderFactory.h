#pragma once

#include "ForwardRenderer.h"
#include <memory>
#include <optional>
#include <string>

struct ShaderFiles;

class ShaderFactory
{
public:
	static shared_ptr<ForwardRenderer> unlit();
	static shared_ptr<ForwardRenderer> phong();
	static shared_ptr<ForwardRenderer> blinnPhong();
	static shared_ptr<ForwardRenderer> reflection();
	static shared_ptr<ForwardRenderer> cubeMap();
	static shared_ptr<ForwardRenderer> interpolative();

private:
	// Instances of singletons for every shader
	static optional<shared_ptr<ForwardRenderer>> unlitShader;
	static optional<shared_ptr<ForwardRenderer>> phongShader;
	static optional<shared_ptr<ForwardRenderer>> blinnPhongShader;
	static optional<shared_ptr<ForwardRenderer>> reflectionShader;
	static optional<shared_ptr<ForwardRenderer>> cubeMapShader;
	static optional<shared_ptr<ForwardRenderer>> interpolativeShader;

	// Paths to the shader files
	static const string SHADER_PATH;
	static const ShaderFiles UNLIT_PATH;
	static const ShaderFiles PHONG_PATH;
	static const ShaderFiles BLINN_PHONG_PATH;
	static const ShaderFiles REFLECTION_PATH;
	static const ShaderFiles CUBE_MAP_PATH;
	static const ShaderFiles INTERPOLATIVE_PATH;
};
