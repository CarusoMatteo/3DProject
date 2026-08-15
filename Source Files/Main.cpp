using namespace std;

#include "../Header Files/Stage.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>

using namespace glm;
using namespace std;

const shared_ptr<fvec3> clearColor(new fvec3(0, 0, 0.16f));
float lastFrameTime = 0.0f;

int main(void)
{
	const unique_ptr<Stage> stage(new Stage(clearColor));

	while (!stage->shouldWindowClose())
	{
		float currentTime = static_cast<float>(glfwGetTime());
		float deltaTime = currentTime - lastFrameTime;

		stage->updateGameObjects(deltaTime);
		stage->drawClearColor();
		stage->renderScene(currentTime);
		stage->swapBuffers();
		stage->pollEvents();

		lastFrameTime = currentTime;
	}

	return 0;
}
