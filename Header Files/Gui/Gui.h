#pragma once

#include "IGui.h"
#include <glm/glm.hpp>

using namespace glm;

/**
 * @brief Represents the main menu GUI of the application.
 */
class Gui : public IGui
{
public:
	Gui(fvec3 *clearColorRef);

	void drawGui() override;
	fvec3 *getClearColorRef() override;

private:
	fvec3 *clearColorRef;

	void settingsWindow(fvec2 position);
};
