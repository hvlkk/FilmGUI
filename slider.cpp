#include "slider.h"

Slider::Slider(const float pos_x, const float pos_y, const float width, const float height, const unsigned int minValue,
	const unsigned int maxValue, const SliderUseage useage) :
	Widget{pos_x, pos_y},
	m_width {width},
	m_height {height},
	m_lineWidth {width * 15.0f},
	m_lowerBound {pos_x - width * 15.0f / 2.0f },		// lower bound = pos_x - slider_line_width / 2
	m_upperBound {pos_x + width * 15.0f / 2.0f },		// upper bound = pos_x + slider_line_width / 2
	m_minValue {minValue},
	m_maxValue {maxValue},
	m_useage {useage},
	m_originalCentre {pos_x}
{
	/* The float value a slider is passed as pos_x, is marked in the private variable "m_originalCentre", and is used
	 * as the centre for the horizontal line we're drawing behind the slider. But, on instantiation, we instantly recalibrate
	 * a slider's default horizontal coordinates, based on whether it is a lower bound slider or an upper bound slider.
	 */

	if (m_useage == SliderUseage::LOWER_BOUND) {
		m_pos_x = m_init_pos_x = m_lowerBound;
	}
	else {
		m_pos_x = m_init_pos_x = m_upperBound;
	}
}

void Slider::draw()
{
	m_brush.outline_opacity = 0.0f;
	SETCOLOUR(m_brush.fill_color, 0.4f, 0.4f, 0.4f);

	// first, drawing a line between the lower and the upper bound of the slider
	graphics::drawRect(m_originalCentre, m_pos_y, m_lineWidth, 9, m_brush);

	// then, drawing two thin lines to serve as visual bounds at the lower/upper bound
	SETCOLOUR(m_brush.fill_color, 0.5f, 0.5f, 0.5f);
	graphics::drawRect(m_upperBound, m_pos_y, 1.2f, m_height / 2, m_brush);
	graphics::drawRect(m_lowerBound, m_pos_y, 1.2f, m_height / 2, m_brush);

	// and now drawing the actual slider
	SETCOLOUR(m_brush.fill_color, 1.0f, 1.0f, 1.0f);
	SETCOLOUR(m_brush.outline_color, 0.25f, 0.25f, 0.25f);

	// drawing it at half the width/height, so dragging it is not as strict
	graphics::drawRect(m_pos_x, m_pos_y, m_width / 2, m_height / 2, m_brush);

	// drawing the min/max values at the respective ends of the slider

	// lower-bound pos_x
	float lb_posX{ m_lowerBound - 32.5f };	// slight offset so they are printed correctly
	graphics::drawText(lb_posX, m_pos_y, 11, std::to_string(m_minValue), m_brush);

	// upper-bound pos_x
	float ub_posX{ m_upperBound + 12.5f };
	graphics::drawText(ub_posX, m_pos_y, 11, std::to_string(m_maxValue), m_brush);

	// then, constantly printing the current value of the slider directly above it:
	graphics::drawText(m_pos_x - m_width / 2 - 2.5f, m_pos_y - m_height / 2 , 11, std::to_string(getCurrentValue()), m_brush);

	// "From:" next to the lower bound slider (minYear), "To:" next to the bottom slider (maxYear)
	if (m_useage == SliderUseage::LOWER_BOUND) {
		graphics::drawText(m_lowerBound - 82.5f, m_pos_y - 20.0f, m_height / 3.0f, "From:", m_brush);
	}
	else {
		graphics::drawText(m_lowerBound - 69.0f, m_pos_y- 20.0f, m_height / 3.0f, "To:", m_brush);
	}

}

void Slider::update()
{
	graphics::MouseState mouse;
	graphics::getMouseState(mouse);
	float mouse_x{ graphics::windowToCanvasX(static_cast<float>(mouse.cur_pos_x)) };
	float mouse_y{ graphics::windowToCanvasY(static_cast<float>(mouse.cur_pos_y)) };

	m_highlighted = contains(mouse_x, mouse_y);

	if (m_highlighted && mouse.dragging) {
		m_pos_x = mouse_x;
		if (mouse_x < m_lowerBound) {
			m_pos_x = m_lowerBound;
		}
		if (mouse_x > m_upperBound) {
			m_pos_x = m_upperBound;
		}
	}
}

// returns the value (in years in our case) the slider's current position signifies
unsigned int Slider::getCurrentValue() const
{
	return m_maxValue - ( (m_maxValue - m_minValue) * ( (m_upperBound - m_pos_x) / (m_upperBound - m_lowerBound) ) );
}

// both sliders active in the search screen will be filtering the results no matter what, therefore making canFilter return true always
bool Slider::canFilter() const
{
	return true;
}

void Slider::filter(std::unordered_set<class Film*>& currFilms) const
{
	auto iter{ currFilms.begin() };
	while (iter != currFilms.end()) {
		// if this slider serves as a lower bound, removing all the films with a release year smaller than the current value of the slider
		if (m_useage == SliderUseage::LOWER_BOUND) {
			if ((*iter)->getReleaseYear() < getCurrentValue()) {
				currFilms.erase(iter++);
				continue;
			}
		}
		// else, if this slider serves as an upper bound, removing all the films with a release year greater than the current value of the slider
		else {
			if ((*iter)->getReleaseYear() > getCurrentValue()) {
				currFilms.erase(iter++);
				continue;
			}
		}
		++iter;
	}
}

// a slider is rectangular, therefore its contains will utilise our "rectangularContains" helper function.
bool Slider::contains(const float x, const float y) const
{
	return rectangularContains(m_pos_x, m_pos_y, m_width, m_height, x, y);
}