#include "../../Header Files/Renderers/Renderer.h"
#include <memory>

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
