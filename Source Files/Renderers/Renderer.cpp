#include "../../Header Files/Renderers/Renderer.h"
#include <glad/glad.h>
#include <iostream>
#include <memory>
#include <stdexcept>

using namespace std;

shared_ptr<bool> Renderer::drawWireframe = make_shared<bool>(false);
shared_ptr<bool> Renderer::drawAnchor = make_shared<bool>(false);

shared_ptr<bool> Renderer::getDrawWireframeFlag()
{
	return Renderer::drawWireframe;
}

shared_ptr<bool> Renderer::getDrawAnchorFlag()
{
	return Renderer::drawAnchor;
}

void Renderer::checkGLErrors()
{
	const unsigned int error = glGetError();
	if (error != GL_NO_ERROR)
	{
		cerr << "OpenGL Error: " << error << endl;
		throw runtime_error("OpenGL encountered an error.");
	}
}
