#include "textbutton.h"
#include <string>


TextButton::TextButton(const float pos_x, const float pos_y, const float width, const float height,
	const std::string& text, const TextButtonUseage useage) :

	Button{ pos_x, pos_y, width, height },
	m_text {text},
	m_useage{useage}
{
}

TextButton::~TextButton()
{
}

void TextButton::draw()
{
	m_brush.fill_opacity = 1.0f;

	// For Apply/Clear Filters, we want a dark grey background with white letters. For Advanced Search, we will not be using a
	// background (it will be transparent), with black letters. We will, also be using that as the default case for "other".
	if (m_useage == TextButtonUseage::APPLY_FILTERS || m_useage == TextButtonUseage::CLEAR_FILTERS) {
		m_brush.outline_opacity = 0.5f * m_highlighted;
		SETCOLOUR(m_brush.outline_color, 0.6f, 0.6f, 0.6f);
		SETCOLOUR(m_brush.fill_color, 0.25f, 0.25f, 0.25f);
		graphics::drawRect(m_pos_x, m_pos_y, m_width, m_height, m_brush);
	}

	// Now, drawing the text:
	if (m_useage == TextButtonUseage::APPLY_FILTERS || m_useage == TextButtonUseage::CLEAR_FILTERS) {
		SETCOLOUR(m_brush.fill_color, 1.0f, 1.0f, 1.0f);
	}
	else {
		SETCOLOUR(m_brush.fill_color, 0.0f, 0.0f, 0.0f);
	}

	setFont("Montserrat-Medium.ttf");

	// slight adjustment to the printing so the text is centered as should be for Clear Filters:
	if (m_useage == TextButtonUseage::CLEAR_FILTERS) {
		graphics::drawText(m_pos_x - m_width / 2.35f, m_pos_y + m_height / 4.0f, m_height * 0.66f, m_text, m_brush);
	}
	else {
		graphics::drawText(m_pos_x - m_width / 2.1f, m_pos_y + m_height / 4.0f, m_height * 0.66f, m_text, m_brush);
	}

	// underlining the text in case the background is transparent
	if (m_useage == TextButtonUseage::ADVANCED_SEARCH || m_useage == TextButtonUseage::OTHER) {
		m_brush.outline_opacity = 1.0f;
		SETCOLOUR(m_brush.outline_color, 0.0f, 0.0f, 0.0f);
		graphics::drawLine(m_pos_x - m_width / 2.1f, m_pos_y + m_height / 3.0f, m_pos_x + m_width / 2.0f, m_pos_y + m_height / 3.0f, m_brush);
	}
}