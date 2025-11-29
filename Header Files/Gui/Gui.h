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
	Gui(const fvec3 *clearColorRef);

	void drawGui() override;
	const fvec3 *getClearColorRef() override;

private:
	const fvec3 *clearColorRef;

	void settingsWindow(const fvec2 position);
};
