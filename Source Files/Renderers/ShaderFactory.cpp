#include "../../Header Files/Renderers/ShaderFactory.h"
#include "../../Header Files/Renderers/ForwardRenderer.h"
#include "../../Header Files/Renderers/ShaderFiles.h"
#include <memory>
#include <optional>
#include <string>

// Paths to the shader files

const string ShaderFactory::SHADER_PATH = "assets/shaders/forward/";
const ShaderFiles ShaderFactory::UNLIT_PATH = {
	ShaderFactory::SHADER_PATH + "Unlit/Unlit.vert",
	ShaderFactory::SHADER_PATH + "Unlit/Unlit.frag"};
const ShaderFiles ShaderFactory::PHONG_PATH = {
	ShaderFactory::SHADER_PATH + "Phong/Phong.vert",
	ShaderFactory::SHADER_PATH + "Phong/Phong.frag"};
const ShaderFiles ShaderFactory::BLINN_PHONG_PATH = {
	ShaderFactory::SHADER_PATH + "Phong/Phong.vert",
	ShaderFactory::SHADER_PATH + "BlinnPhong/BlinnPhong.frag"};
const ShaderFiles ShaderFactory::REFLECTION_PATH = {
	ShaderFactory::SHADER_PATH + "Reflection/Reflection.vert",
	ShaderFactory::SHADER_PATH + "Reflection/Reflection.frag"};
const ShaderFiles ShaderFactory::CUBE_MAP_PATH = {
	ShaderFactory::SHADER_PATH + "CubeMap/CubeMap.vert",
	ShaderFactory::SHADER_PATH + "CubeMap/CubeMap.frag"};
const ShaderFiles ShaderFactory::INTERPOLATIVE_PATH = {
	ShaderFactory::SHADER_PATH + "Interpolative/Interpolative.vert",
	ShaderFactory::SHADER_PATH + "Interpolative/Interpolative.frag"};

// Shader singletons

optional<shared_ptr<ForwardRenderer>> ShaderFactory::unlitShader = nullopt;
optional<shared_ptr<ForwardRenderer>> ShaderFactory::phongShader = nullopt;
optional<shared_ptr<ForwardRenderer>> ShaderFactory::blinnPhongShader = nullopt;
optional<shared_ptr<ForwardRenderer>> ShaderFactory::reflectionShader = nullopt;
optional<shared_ptr<ForwardRenderer>> ShaderFactory::cubeMapShader = nullopt;
optional<shared_ptr<ForwardRenderer>> ShaderFactory::interpolativeShader = nullopt;

shared_ptr<ForwardRenderer> ShaderFactory::unlit()
{
	if (!unlitShader.has_value())
		unlitShader = make_shared<ForwardRenderer>(UNLIT_PATH);
	return unlitShader.value();
}

shared_ptr<ForwardRenderer> ShaderFactory::phong()
{
	if (!phongShader.has_value())
		phongShader = make_shared<ForwardRenderer>(PHONG_PATH);
	return phongShader.value();
}

shared_ptr<ForwardRenderer> ShaderFactory::blinnPhong()
{
	if (!blinnPhongShader.has_value())
		blinnPhongShader = make_shared<ForwardRenderer>(BLINN_PHONG_PATH);
	return blinnPhongShader.value();
}

shared_ptr<ForwardRenderer> ShaderFactory::reflection()
{
	if (!reflectionShader.has_value())
		reflectionShader = make_shared<ForwardRenderer>(REFLECTION_PATH);
	return reflectionShader.value();
}

shared_ptr<ForwardRenderer> ShaderFactory::cubeMap()
{
	if (!cubeMapShader.has_value())
		cubeMapShader = make_shared<ForwardRenderer>(CUBE_MAP_PATH);
	return cubeMapShader.value();
}

shared_ptr<ForwardRenderer> ShaderFactory::interpolative()
{
	if (!interpolativeShader.has_value())
		interpolativeShader = make_shared<ForwardRenderer>(INTERPOLATIVE_PATH);
	return interpolativeShader.value();
}
