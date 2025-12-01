#pragma once

#include "IGui.h"
#include <glm/glm.hpp>
#include <memory>

using namespace glm;

/**
 * @brief Represents the main menu GUI of the application.
 */
class Gui : public IGui
{
public:
	Gui(const shared_ptr<fvec3> clearColor);

	void drawGui() override;
	const shared_ptr<fvec3> getClearColor() override;

private:
	const fvec2 settingsWindowPosition = fvec2(10, 10);

	shared_ptr<fvec3> clearColor;

	void settingsWindow();
};
