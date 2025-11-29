#include "../Header Files/Window.h"

Window::Window(string windowTitle)
{
}

bool Window::shouldWindowClose()
{
    return false;
}

void Window::swapBuffers()
{
}

void Window::pollEvents()
{
}

ivec2 Window::getWindowSize()
{
    return ivec2();
}

void Window::initializeWindow(string windowTitle, double monitorPercentageWidth, double monitorPercentageHeight)
{
}

void Window::initInputEvents()
{
}

void Window::initializeGui()
{
}

void Window::initOpenGL()
{
}

fmat4 Window::createProjectionMatrix(int windowWidth, int windowHeight)
{
    return fmat4();
}
