#pragma once

#include <glm/glm.hpp>

using namespace glm;

/**
 * @brief Interface for GUI components in the application.
 */
class IGui
{
public:
	virtual ~IGui() = default;
	/// @brief Draws the GUI elements.
	virtual void drawGui() = 0;
	virtual const fvec3 *getClearColorRef() = 0;
};
