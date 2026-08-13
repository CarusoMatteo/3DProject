#include "../../Header Files/Renderers/ShaderFactory.h"
#include "../../Header Files/Renderers/ForwardRenderer.h"
#include "../../Header Files/Renderers/GeometryRenderer.h"
#include "../../Header Files/Renderers/ShaderFiles.h"
#include <memory>
#include <optional>
#include <string>

optional<shared_ptr<GeometryRenderer>> ShaderFactory::geometryShader = nullopt;

// Paths to the shader files

const string FORWARD_SHADER_PATH = "assets/shaders/forward/";
const ShaderFiles UNLIT_PATH = {
	FORWARD_SHADER_PATH + "Unlit/Unlit.vert",
	FORWARD_SHADER_PATH + "Unlit/Unlit.frag"};
const ShaderFiles PHONG_PATH = {
	FORWARD_SHADER_PATH + "Phong/Phong.vert",
	FORWARD_SHADER_PATH + "Phong/Phong.frag"};
const ShaderFiles BLINN_PHONG_PATH = {
	FORWARD_SHADER_PATH + "Phong/Phong.vert",
	FORWARD_SHADER_PATH + "BlinnPhong/BlinnPhong.frag"};
const ShaderFiles REFLECTION_PATH = {
	FORWARD_SHADER_PATH + "Reflection/Reflection.vert",
	FORWARD_SHADER_PATH + "Reflection/Reflection.frag"};
const ShaderFiles CUBE_MAP_PATH = {
	FORWARD_SHADER_PATH + "CubeMap/CubeMap.vert",
	FORWARD_SHADER_PATH + "CubeMap/CubeMap.frag"};
const ShaderFiles INTERPOLATIVE_PATH = {
	FORWARD_SHADER_PATH + "Interpolative/Interpolative.vert",
	FORWARD_SHADER_PATH + "Interpolative/Interpolative.frag"};

const string DEFERRED_SHADER_PATH = "assets/shaders/deferred/";
const ShaderFiles FBO_DEBUG_PATH = {
	DEFERRED_SHADER_PATH + "FboDebug/FboDebug.vert",
	DEFERRED_SHADER_PATH + "FboDebug/FboDebug.frag"};
const ShaderFiles GEOMETRY_PASS_PATH = {
	DEFERRED_SHADER_PATH + "GeometryPass/GeometryPass.vert",
	DEFERRED_SHADER_PATH + "GeometryPass/GeometryPass.frag"};
const ShaderFiles LIGHT_BOX_PATH = {
	DEFERRED_SHADER_PATH + "LightBox/LightBox.vert",
	DEFERRED_SHADER_PATH + "LightBox/LightBox.frag"};
const ShaderFiles LIGHTING_PASS_PATH = {
	DEFERRED_SHADER_PATH + "LightingPass/LightingPass.vert",
	DEFERRED_SHADER_PATH + "LightingPass/LightingPass.frag"};

shared_ptr<ForwardRenderer> ShaderFactory::unlit()
{
	return make_shared<ForwardRenderer>(UNLIT_PATH);
}

shared_ptr<ForwardRenderer> ShaderFactory::phong()
{
	return make_shared<ForwardRenderer>(PHONG_PATH);
}

shared_ptr<ForwardRenderer> ShaderFactory::blinnPhong()
{
	return make_shared<ForwardRenderer>(BLINN_PHONG_PATH);
}

shared_ptr<ForwardRenderer> ShaderFactory::reflection()
{
	return make_shared<ForwardRenderer>(REFLECTION_PATH);
}

shared_ptr<ForwardRenderer> ShaderFactory::cubeMap()
{
	return make_shared<ForwardRenderer>(CUBE_MAP_PATH);
}

shared_ptr<ForwardRenderer> ShaderFactory::interpolative()
{
	return make_shared<ForwardRenderer>(INTERPOLATIVE_PATH);
}

shared_ptr<GeometryRenderer> ShaderFactory::geometry()
{
	if (!geometryShader.has_value())
		geometryShader = make_shared<GeometryRenderer>(GEOMETRY_PASS_PATH, LIGHTING_PASS_PATH);
	return geometryShader.value();
}
