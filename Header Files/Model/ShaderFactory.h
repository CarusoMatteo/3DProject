#pragma once

#include "Shader.h"
#include <memory>
#include <optional>
#include <string>

struct ShaderFiles;

class ShaderFactory
{
public:
	static shared_ptr<Shader> unlit();
	static shared_ptr<Shader> phong();
	static shared_ptr<Shader> blinnPhong();
	static shared_ptr<Shader> reflection();
	static shared_ptr<Shader> cubeMap();
	static shared_ptr<Shader> interpolative();

private:
	// Instances of singletons for every shader
	static optional<shared_ptr<Shader>> unlitShader;
	static optional<shared_ptr<Shader>> phongShader;
	static optional<shared_ptr<Shader>> blinnPhongShader;
	static optional<shared_ptr<Shader>> reflectionShader;
	static optional<shared_ptr<Shader>> cubeMapShader;
	static optional<shared_ptr<Shader>> interpolativeShader;

	// Paths to the shader files
	static const string SHADER_PATH;
	static const ShaderFiles UNLIT_PATH;
	static const ShaderFiles PHONG_PATH;
	static const ShaderFiles BLINN_PHONG_PATH;
	static const ShaderFiles REFLECTION_PATH;
	static const ShaderFiles CUBE_MAP_PATH;
	static const ShaderFiles INTERPOLATIVE_PATH;
};
