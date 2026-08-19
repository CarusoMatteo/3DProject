#include "../../Header Files/Renderers/Renderer.h"
#include "../../Header Files/Renderers/RenderMode.h"
#include <glad/glad.h>
#include <iostream>
#include <memory>
#include <stdexcept>

using namespace std;

shared_ptr<bool> Renderer::drawWireframe = make_shared<bool>(false);
shared_ptr<bool> Renderer::drawAnchor = make_shared<bool>(false);
shared_ptr<RenderMode> Renderer::renderMode = make_shared<RenderMode>(RenderMode::MAIN);

shared_ptr<bool> Renderer::getDrawWireframeFlag()
{
	return Renderer::drawWireframe;
}

shared_ptr<bool> Renderer::getDrawAnchorFlag()
{
	return Renderer::drawAnchor;
}

void Renderer::setRenderMode(const RenderMode mode)
{
	*Renderer::renderMode = mode;
}

bool Renderer::bufferIsUsed(const unsigned int address, const size_t size) const
{
	return address != -1 && size > 0;
}

void Renderer::checkGLErrors() const
{
	const unsigned int error = glGetError();
	if (error != GL_NO_ERROR)
	{
		cerr << "OpenGL Error: " << error << endl;
		throw runtime_error("OpenGL encountered an error.");
	}
}
