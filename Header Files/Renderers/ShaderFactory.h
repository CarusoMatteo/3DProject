#pragma once

#include "DeferredRenderer.h"
#include "ForwardRenderer.h"
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

	static shared_ptr<DeferredRenderer> fboDebug();
	static shared_ptr<DeferredRenderer> geometryPass();
	static shared_ptr<DeferredRenderer> lightBox();

	// TODO: What type should this return? Maybe its own type, like `LightingPassRenderer`?
	//  In that case, geometryPassShader's type (DeferredRenderer) should probably be renamed to `GeometryPassRenderer`.
	static shared_ptr<ForwardRenderer> lightingPass();

private:
	// Instances of singletons for every shader
	static optional<shared_ptr<ForwardRenderer>> unlitShader;
	static optional<shared_ptr<ForwardRenderer>> phongShader;
	static optional<shared_ptr<ForwardRenderer>> blinnPhongShader;
	static optional<shared_ptr<ForwardRenderer>> reflectionShader;
	static optional<shared_ptr<ForwardRenderer>> cubeMapShader;
	static optional<shared_ptr<ForwardRenderer>> interpolativeShader;

	static optional<shared_ptr<DeferredRenderer>> fboDebugShader;
	static optional<shared_ptr<DeferredRenderer>> geometryPassShader;
	static optional<shared_ptr<DeferredRenderer>> lightBoxShader;
	static optional<shared_ptr<ForwardRenderer>> lightingPassShader;
};
