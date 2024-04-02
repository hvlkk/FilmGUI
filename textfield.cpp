#include "textfield.h"
#include "widget.h"

#include <string>
#include <algorithm>	// for std::transform


/* offsetString: Implementing a custom condition based on the size of the text we're printing:
*  If the size of the string is smaller than width / 12.0f, draw the whole string. Otherwise,
*  draw the last (width / 12.0f) characters of the string. To prevent text escaping our bounds. */
const std::string TextField::offsetString() const
{
	// size is size_t == unsigned long int, therefore casting the result of width to unsigned long int
	if (m_string.size() < static_cast<unsigned long int>(m_width / 12.0f)) {
		return m_string;
	}
	else {
		size_t currentStartPoint{ m_string.size() - static_cast<unsigned long int>(m_width / 12.0f) };
		return m_string.substr(currentStartPoint);
	}
}

TextField::TextField(const float pos_x, const float pos_y, const float width, const float height, const TextfieldUseage useage) :
	Widget{pos_x, pos_y},
	m_width{width},
	m_height{height},
	m_useage{useage},

	// Going to be taking the rightmost ~1/8th of the textfield to represent the x button, therefore calculating its coordinates accordingly.
	m_button{pos_x + width * (7.35f/16.0f), pos_y + height * 0.05f, height * 0.55f, height * 0.55f, "x.png", TextureButtonUseage::TEXTFIELD_X}
{
}

void TextField::draw()
{
	float highlighted{ 0.2f + m_highlighted * 0.2f };
	float clicked{ 0.6f * m_clicked };

	// if the textfield has been clicked on, make it maintain a 1.0f outline_color value
	if (m_clicked) {
		SETCOLOUR(m_brush.outline_color, clicked, clicked, clicked);
	}
	else {
		SETCOLOUR(m_brush.outline_color, highlighted, highlighted, highlighted);
	}

	m_brush.outline_opacity = 1.0f;
	SETCOLOUR(m_brush.fill_color, 0.35f, 0.35f, 0.35f);
	graphics::drawRect(m_pos_x, m_pos_y, m_width, m_height, m_brush);

	// printing the "purpose" of the respective TextField next to it:
	SETCOLOUR(m_brush.fill_color, 1.0f, 1.0f, 1.0f);
	if (m_useage == TextfieldUseage::ACTOR) {
		graphics::drawText(m_pos_x - m_width * 0.825f, m_pos_y + m_height / 4.0f, m_height / 2.0f, "Actor", m_brush);
	}
	else if (m_useage == TextfieldUseage::DIRECTOR) {
		graphics::drawText(m_pos_x - m_width * 0.825f, m_pos_y + m_height / 4.0f, m_height / 2.0f, "Director", m_brush);
	}
	else if (m_useage == TextfieldUseage::TITLE) {
		graphics::drawText(m_pos_x - m_width * 0.825f, m_pos_y + m_height /4.0f , m_height / 2.0f, "Title", m_brush);
	}

	// drawing the button
	m_button.draw();

	SETCOLOUR(m_brush.fill_color, 0.75f, 0.75f, 0.75f);
	m_brush.outline_opacity = 0.0f;
	graphics::drawText(m_pos_x - m_width / 2.175f, m_pos_y + m_height / 4, 14.5f, offsetString(), m_brush);
}

void TextField::update()
{
	graphics::MouseState mouse;
	graphics::getMouseState(mouse);
	float mouse_x{ graphics::windowToCanvasX(static_cast<float>(mouse.cur_pos_x)) };
	float mouse_y{ graphics::windowToCanvasY(static_cast<float>(mouse.cur_pos_y)) };

	m_button.update();

	// we switch the "clicked" state to true if we click inside the textfield
	m_highlighted = contains(mouse_x, mouse_y);
	if (m_highlighted && mouse.button_left_pressed) {
		m_clicked = true;
	}

	if (m_clicked) {
		// if we have clicked on the textfield and we click anywhere outside it, make "clicked" false
		if (!m_highlighted && mouse.button_left_pressed) {
			m_clicked = false;
		}

		static float delay{ 0.0f };					// used to ensure enough time has passed since the last time we typed a character
		delay += graphics::getDeltaTime();			// incrementing delay by the time that's passed since our last update function

		// first, checking all the letters from a to z through a for loop.
		for (int i { graphics::SCANCODE_A }; i <= graphics::SCANCODE_Z; ++i) {
			// if the currently iterated key has been pressed
			if (graphics::getKeyState(static_cast<graphics::scancode_t>(i))) {

				// if the previous key pressed is the same and the delay is smaller than 140.0f, ignore and continue the loop
				if (delay < 125.0f) {
					continue;
				}
				else {
					delay = 0.0f;
					// pushing back the ascii value of the letter
					m_string.push_back(i + 93);
				}
			}
		}

		// then, checking all numbers from 1 to 9
		for (int i{ graphics::SCANCODE_1 }; i <= graphics::SCANCODE_9; ++i) {

			// if the currently iterated key has been pressed
			if (graphics::getKeyState(static_cast<graphics::scancode_t>(i))) {

				// if the previous key pressed is the same and the delay is smaller than 140.0f, ignore and continue the loop
				if (delay < 125.0f) {
					continue;
				}
				else {
					delay = 0.0f;
					// pushing back the ascii value of the number
					m_string.push_back(i + 19);
				}
			}
		}

		// then, checking for 0
		if (graphics::getKeyState(graphics::SCANCODE_0)) {
			if (delay >= 125.0f) {
				delay = 0.0f;
				// pushing back the ascii value of the number
				m_string.push_back(graphics::SCANCODE_0 + 9);
			}
		}

		// then, checking for backspace
		if (graphics::getKeyState(graphics::SCANCODE_BACKSPACE)) {
			if (delay >= 125.0f) {
				delay = 0.0f;
				if (m_string.size() >= 1) {
					m_string.pop_back();
				}
			}
		}

		// and likewise, checking for space
		if (graphics::getKeyState(graphics::SCANCODE_SPACE)) {
			if (delay >= 125.0f) {
				delay = 0.0f;
				m_string.push_back(' ');
			}
		}

		// now, checking if we clicked on the x button: if we did, reset the textfield, and reset the x button
		if (m_button.isClicked()) {
			resetState();
			m_button.resetState();
		}
	}
}

void TextField::resetState()
{
	Widget::resetState();
	m_string.clear();
}

// a textfield is rectangular, therefore its contains will utilise our "rectangularContains" helper function.
bool TextField::contains(const float x, const float y) const
{
	return rectangularContains(m_pos_x, m_pos_y, m_width, m_height, x, y);
}

// Textfields will only be allowed to filter if they are not empty.
bool TextField::canFilter() const
{
	return !m_string.empty();
}

void TextField::filter(std::unordered_set<class Film*>& currFilms) const
{
	auto iter{ currFilms.begin() };
	while (iter != currFilms.end()) {

		// if this textfield filters based on an actor's name, remove all the films that don't contain this actor's name in their cast
		if (m_useage == TextfieldUseage::ACTOR) {

			/* Using the castToString() function we also use to draw a film's cast.
			 * Then, since the cast is correctly capitalised, using std::transform to
			 * make the entire string lowercase, so that std::find is accurate. */

			std::string entireCast{ (*iter)->castToString() };
			std::transform(entireCast.begin(), entireCast.end(), entireCast.begin(), std::tolower);
			if (entireCast.find(m_string) == std::string::npos) {
				currFilms.erase(iter++);
				continue;
			}
		}
		// else, if this textfield filters based on an director's name, remove all the films that don't have this person listed as the director
		else if (m_useage == TextfieldUseage::DIRECTOR) {
			std::string director{ (*iter)->getDirector() };
			std::transform(director.begin(), director.end(), director.begin(), std::tolower);
			if (director.find(m_string) == std::string::npos) {
				currFilms.erase(iter++);
				continue;
			}
		}
		// likewise if this textfield filters based on a film's title
		else if (m_useage == TextfieldUseage::TITLE) {
			std::string title{ (*iter)->getTitle() };
			std::transform(title.begin(), title.end(), title.begin(), std::tolower);
			if (title.find(m_string) == std::string::npos) {
				currFilms.erase(iter++);
				continue;
			}
		}
		// and likewise, if the textfield filters based on everything (the main menu textfield):
		else {
			// Since we will only be erasing a film from our list if it fails all 3 conditions, we will use a boolean variable.
			bool toErase = true;
			std::string director{ (*iter)->getDirector() };
			std::transform(director.begin(), director.end(), director.begin(), std::tolower);
			std::string entireCast{ (*iter)->castToString() };
			std::transform(entireCast.begin(), entireCast.end(), entireCast.begin(), std::tolower);
			std::string title{ (*iter)->getTitle() };
			std::transform(title.begin(), title.end(), title.begin(), std::tolower);

			// first, checking the title. If a title contains the word typed, make toErase false.
			if (title.find(m_string) != std::string::npos) {
				toErase = false;
			}

			// introducing toErase in our last 2 tests: if a film has already passed our first check, no need to check for the following 2
			if (toErase && director.find(m_string) != std::string::npos) {
				toErase = false;
			}
			if (toErase && entireCast.find(m_string) != std::string::npos) {
				toErase = false;
			}

			// if the film passed none of our checks, erase the film from the set.
			if (toErase) {
				currFilms.erase(iter++);
				continue;
			}
		}
		++iter;
	}
}
