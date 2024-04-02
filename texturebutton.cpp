#include "texturebutton.h"

TextureButton::TextureButton(const float pos_x, const float pos_y, const float width, const float height,
	const std::string& fileName, const TextureButtonUseage useage) :

	Button{pos_x, pos_y, width, height},
	m_texture{fileName},
	m_useage{useage}
{
}

TextureButton::~TextureButton()
{
}

void TextureButton::draw()
{
	// All the texture buttons we have included so far will be drawn in a transparent background.
	setTexture(m_brush, m_texture);
	m_brush.outline_opacity = 0.0f;
	if (m_useage == TextureButtonUseage::OTHER || m_useage == TextureButtonUseage::DEFAULT) {
		m_brush.fill_opacity = 1.0f;
	} else if (m_useage == TextureButtonUseage::TEXTFIELD_X) {
		m_brush.fill_opacity = 0.65f + m_highlighted * 0.35f;
	}
	SETCOLOUR(m_brush.fill_color, 1.0f, 1.0f, 1.0f);
	graphics::drawRect(m_pos_x, m_pos_y, m_width, m_height, m_brush);
}

void TextureButton::update()
{
	// if the button is the undo button or an "other" button, utilise the typical Button update:
	if (m_useage == TextureButtonUseage::OTHER || m_useage == TextureButtonUseage::DEFAULT) {
		Button::update();
	}
	// else, if this is a textfield x button, do not use the clicking sound, but do everything else as normal.
	else if (m_useage == TextureButtonUseage::TEXTFIELD_X) {
		graphics::MouseState mouse;
		graphics::getMouseState(mouse);
		float mouse_x{ graphics::windowToCanvasX(static_cast<float>(mouse.cur_pos_x)) };
		float mouse_y{ graphics::windowToCanvasY(static_cast<float>(mouse.cur_pos_y)) };

		m_highlighted = contains(mouse_x, mouse_y);

		if (m_highlighted && mouse.button_left_pressed) {
			m_clicked = !m_clicked;
		}
	}
}
