#pragma once
#include "widget.h"

/* Button: The class representing buttons. Split into TextButton and TextureButton, depending on whether a button will have
 * some text it needs to disaplay, or some image it needs to display. No button classes will be directly instantiated. 
 *
 * Also, making the assumption all Buttons will be rectangular, therefore including a width and height variable to Button.
 */

class Button : public Widget {
protected:
	float m_width;
	float m_height;

public:
	Button(const float pos_x, const float pos_y, const float width = 5.0f, const float height = 5.0f) :
		Widget {pos_x, pos_y},
		m_width {width},
		m_height {height}
	{}
	virtual ~Button() {}

	virtual float getHeight() const { return m_height; }
	virtual float getWidth() const { return m_width; }


	virtual bool contains(float x, float y) const override { return rectangularContains(m_pos_x, m_pos_y, m_width, m_height, x, y); }

	// Same case for all the buttons:
	// Update plays the sound of the click of a button; if we click on a button, we make m_clicked the opposite of what it used to be.
	virtual void update() override {
		graphics::MouseState mouse;
		graphics::getMouseState(mouse);
		float mouse_x{ graphics::windowToCanvasX(static_cast<float>(mouse.cur_pos_x)) };
		float mouse_y{ graphics::windowToCanvasY(static_cast<float>(mouse.cur_pos_y)) };

		m_highlighted = contains(mouse_x, mouse_y);

		if (m_highlighted && mouse.button_left_pressed) {
			graphics::playSound(static_cast<std::string>(ASSET_PATH) + "button.wav", 1.0f);
			m_clicked = !m_clicked;
		}
	}
};