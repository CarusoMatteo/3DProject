#include "../../Header Files/Renderers/ShaderFactory.h"
#include "../../Header Files/Renderers/ForwardRenderer.h"
#include "../../Header Files/Renderers/ShaderFiles.h"
#include <memory>
#include <optional>
#include <string>

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

// Shader singletons

optional<shared_ptr<ForwardRenderer>> ShaderFactory::unlitShader = nullopt;
optional<shared_ptr<ForwardRenderer>> ShaderFactory::phongShader = nullopt;
optional<shared_ptr<ForwardRenderer>> ShaderFactory::blinnPhongShader = nullopt;
optional<shared_ptr<ForwardRenderer>> ShaderFactory::reflectionShader = nullopt;
optional<shared_ptr<ForwardRenderer>> ShaderFactory::cubeMapShader = nullopt;
optional<shared_ptr<ForwardRenderer>> ShaderFactory::interpolativeShader = nullopt;

optional<shared_ptr<DeferredRenderer>> ShaderFactory::fboDebugShader = nullopt;
optional<shared_ptr<DeferredRenderer>> ShaderFactory::geometryPassShader = nullopt;
optional<shared_ptr<DeferredRenderer>> ShaderFactory::lightBoxShader = nullopt;
optional<shared_ptr<ForwardRenderer>> ShaderFactory::lightingPassShader = nullopt;

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

shared_ptr<DeferredRenderer> ShaderFactory::fboDebug()
{
	if (!fboDebugShader.has_value())
		fboDebugShader = make_shared<DeferredRenderer>(FBO_DEBUG_PATH);
	return fboDebugShader.value();
}

shared_ptr<DeferredRenderer> ShaderFactory::geometryPass()
{
	if (!geometryPassShader.has_value())
		geometryPassShader = make_shared<DeferredRenderer>(GEOMETRY_PASS_PATH);
	return geometryPassShader.value();
}

shared_ptr<DeferredRenderer> ShaderFactory::lightBox()
{
	if (!lightBoxShader.has_value())
		lightBoxShader = make_shared<DeferredRenderer>(LIGHT_BOX_PATH);
	return lightBoxShader.value();
}

shared_ptr<ForwardRenderer> ShaderFactory::lightingPass()
{
	if (!lightingPassShader.has_value())
		lightingPassShader = make_shared<ForwardRenderer>(LIGHTING_PASS_PATH);
	return lightingPassShader.value();
}
