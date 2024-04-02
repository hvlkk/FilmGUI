#pragma once
#include "global.h"
#include "graphics.h"
#include <string_view>

/* util.h: Contains multiple helper functions. */

// SETCOLOUR: Used to set a brush's colour in a single line
#define SETCOLOUR(colour, r, g, b) {colour[0] = r; colour[1] = g; colour[2] = b;}

// for whenever we need to portray something at a specific location in our canvas
inline float canvasWidthOffset(float f) { return CANVAS_WIDTH * f; }
inline float canvasHeightOffset(float f) { return CANVAS_HEIGHT * f; }

// to set the font
inline void setFont(std::string_view font) { graphics::setFont(ASSET_PATH + static_cast<std::string>(font)); }

// to set the texture of a brush
inline void setTexture(graphics::Brush& brush, const std::string& fileName) { brush.texture = static_cast<std::string>(ASSET_PATH) + fileName; }

// to check if a rectangular widget contains the coordinates our mouse is hovering over. pos_x and pos_y refer to the centre of the item
inline bool rectangularContains(const float pos_x, const float pos_y, const float itemWidth, const float itemHeight, const float mouse_x, const float mouse_y)
{
	return mouse_x > pos_x - itemWidth / 2 && mouse_x < pos_x + itemWidth / 2 && mouse_y > pos_y - itemHeight / 2 && mouse_y < pos_y + itemHeight / 2;
}

// DEPRECATED: Not used in this project.
inline float distance(float x1, float y1, float x2, float y2)
{
	float dx = x1 - x2;
	float dy = y1 - y2;
	return sqrtf(dx * dx + dy * dy);
}