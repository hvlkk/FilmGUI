#include "film.h"
#include "filmutil.h"
#include "util.h"
#include "global.h"
#include "graphics.h"

#include <unordered_set>
#include <array>
#include <string>
#include <iostream>
#include <cstdio>
#include <vector>

//initialising all member variables
Film::Film(const std::string_view title, unsigned int releaseYear, const std::string_view director,
	const std::unordered_set<Genre>& genres, const std::array<std::string, 5>& cast,
	const std::string_view description, const std::string_view poster) :
	m_title{title},
	m_releaseYear{releaseYear},
	m_director {director},
	m_genres{ genres },
	m_cast{ cast },
	m_description{description},
	m_poster{ poster },
	m_filmID{ s_filmID++ },

	// utilising modulo to generate an offset for the starting position based on the id of the film:
	// modulo 5 to calculate the starting x location (10 films, 2 rows)
	// and modulo 2 to calculate the starting y location (10 films, 5 cols)
	m_pos_x{ canvasWidthOffset( ( (m_filmID % 5) / 5.0f) + 0.1f) },
	m_pos_y{ canvasHeightOffset( ( (m_filmID % 2) / 3.0f) + 0.2f) }
{
}

Film::~Film() = default;

void Film::draw()
{
	float highlighted = 0.45f * m_highlighted;
	float active = 1.0f * m_active;
	m_brush.outline_opacity = 1.0f;

	// if we haven't yet clicked on a film, therefore every film we hover over will have its relevant info shown at the bottom
	if (s_filmState == FilmState::NO_FILM_CLICKED) {
		SETCOLOUR(m_brush.outline_color, highlighted, highlighted, highlighted);
		setTexture(m_brush, m_poster);
		graphics::drawRect(m_pos_x, m_pos_y, m_posterWidth, m_posterHeight, m_brush);

		// if highlighted, display the relevant info at the bottom of the page
		if (m_highlighted) {

			setFont("Montserrat-Medium.ttf");
			SETCOLOUR(m_brush.fill_color, 1.0f, 1.0f, 1.0f);
			graphics::drawText(canvasWidthOffset(0.025f), canvasHeightOffset(0.735f), 40, m_title, m_brush);		// title

			// genres

			/* Drawing the genres starting from canvasWidthOffset(0.65f). Each genre will be incrementing
			 * the current offset based on the size of their respective names.	*/

			float previousOffset{ 0.65f };

			setFont("Montserrat-MediumItalic.ttf");
			SETCOLOUR(m_brush.fill_color, 0.7f, 0.6f, 0.0f);

			for (const auto genre : m_genres) {
				graphics::drawText(canvasWidthOffset(previousOffset), canvasHeightOffset(0.735f), 18, genreName(genre), m_brush);
				if (genreName(genre).size() <= 5) {
					previousOffset += 0.06f + genreName(genre).size() * 0.0025f;
				}
				else {
					previousOffset += 0.06f + genreName(genre).size() * 0.005f;
				}
			}

			setFont("Montserrat-Medium.ttf");
			SETCOLOUR(m_brush.fill_color, 1.0f, 1.0f, 1.0f);

			drawLineOfText(0.025f, 0.78f, 25, "Director:", m_director);												// director
			drawLineOfText(0.025f, 0.82f, 23, "Cast:", castToString());												// cast
			drawLineOfText(0.025f, 0.86f, 23, "Year:", std::to_string(m_releaseYear));								// year

			std::vector<std::string> description{ tokenizeDescription()};
			SETCOLOUR(m_brush.fill_color, 0.5f, 0.5f, 0.5f);
			setFont("Montserrat-MediumItalic.ttf");
			graphics::drawText(canvasWidthOffset(0.025f), canvasHeightOffset(0.90f), 23, "Summary:", m_brush);
			SETCOLOUR(m_brush.fill_color, 1.0f, 1.0f, 1.0f);
			setFont("Montserrat-Medium.ttf");
			for (int i{ 0 }; i < description.size(); ++i) {
				graphics::drawText(canvasWidthOffset(0.15f), canvasHeightOffset(0.90f + (i * 0.03f)), 21, description[i], m_brush);
			}
		}
	}
	// if film_state_t == FILM_CLICKED, therefore only the active film will have its relevant info shown at the bottom
	else {
		highlighted -= 0.15f * m_highlighted;
		if (m_active) {
			SETCOLOUR(m_brush.outline_color, active, active, active);
			setTexture(m_brush, m_poster);
			graphics::drawRect(m_pos_x, m_pos_y, m_posterWidth, m_posterHeight, m_brush);

			setFont("Montserrat-Medium.ttf");
			SETCOLOUR(m_brush.fill_color, 1.0f, 1.0f, 1.0f);
			graphics::drawText(canvasWidthOffset(0.025f), canvasHeightOffset(0.735f), 40, m_title, m_brush);		// title

			// genres

			/* Drawing the genres starting from canvasWidthOffset(0.65f). Each genre will be incrementing
			 * the current offset based on the size of their respective names.	*/

			float previousOffset{ 0.65f };

			setFont("Montserrat-MediumItalic.ttf");
			SETCOLOUR(m_brush.fill_color, 0.7f, 0.6f, 0.0f);

			for (const auto genre : m_genres) {
				graphics::drawText(canvasWidthOffset(previousOffset), canvasHeightOffset(0.735f), 18, genreName(genre), m_brush);
				if (genreName(genre).size() <= 5) {
					previousOffset += 0.06f + genreName(genre).size() * 0.0025f;
				}
				else {
					previousOffset += 0.06f + genreName(genre).size() * 0.005f;
				}
			}

			setFont("Montserrat-Medium.ttf");
			SETCOLOUR(m_brush.fill_color, 1.0f, 1.0f, 1.0f);

			drawLineOfText(0.025f, 0.78f, 25, "Director:", m_director);												// director
			drawLineOfText(0.025f, 0.82f, 23, "Cast:", castToString());												// cast
			drawLineOfText(0.025f, 0.86f, 23, "Year:", std::to_string(m_releaseYear));								// year

			std::vector<std::string> description{ tokenizeDescription() };
			SETCOLOUR(m_brush.fill_color, 0.5f, 0.5f, 0.5f);
			setFont("Montserrat-MediumItalic.ttf");
			graphics::drawText(canvasWidthOffset(0.025f), canvasHeightOffset(0.90f), 23, "Summary:", m_brush);
			SETCOLOUR(m_brush.fill_color, 1.0f, 1.0f, 1.0f);
			setFont("Montserrat-Medium.ttf");
			for (int i{ 0 }; i < description.size(); ++i) {
				graphics::drawText(canvasWidthOffset(0.15f), canvasHeightOffset(0.90f + (i * 0.03f)), 21, description[i], m_brush);
			}
		}
		// active films have twice as intense an outline as highlighted films do. Therefore, making this condition an else, so
		// if we have set a film as active, we do not bother drawing it with the active outline, but with the highlighted outline
		else {
			// if a film is highlighted while we do have an active film, give it a slight outline
			SETCOLOUR(m_brush.outline_color, highlighted, highlighted, highlighted);
			setTexture(m_brush, m_poster);
			graphics::drawRect(m_pos_x, m_pos_y, m_posterWidth, m_posterHeight, m_brush);
		}
	}

	//resetting the film brush
	SETCOLOUR(m_brush.outline_color, 0.0f, 0.0f, 0.0f);
	setTexture(m_brush, "");

}


void Film::drawLineOfText(float offsetX, float offsetY, float size, const std::string& attribute, const std::string& text)
{
	// printing the attribute towards the left of the screen
	SETCOLOUR(m_brush.fill_color, 0.5f, 0.5f, 0.5f);
	setFont("Montserrat-MediumItalic.ttf");
	graphics::drawText(canvasWidthOffset(offsetX), canvasHeightOffset(offsetY), size, attribute, m_brush);

	// updating offsets, slightly lowering the size of the text to be printed now
	offsetX += 0.1275f;
	size *= 0.9f;

	//printing the text towards the centre of the screen
	SETCOLOUR(m_brush.fill_color, 1.0f, 1.0f, 1.0f);
	setFont("Montserrat-Medium.ttf");
	graphics::drawText(canvasWidthOffset(offsetX), canvasHeightOffset(offsetY), size , text, m_brush);
}

// DEPRECATED: Film's update is not used in this project.
void Film::update()
{
}

// DEPRECATED
void Film::setPosX(const float pos_x)
{
	m_pos_x = pos_x;
}

// DEPRECATED
void Film::setPosY(const float pos_y)
{
	m_pos_y = pos_y;
}

// to check if our mouse coordinates are inside a film poster
bool Film::contains(const float x, const float y) const
{
	return rectangularContains(m_pos_x, m_pos_y, m_posterWidth, m_posterHeight, x, y);
}

// for when we have to print the cast to screen
std::string Film::castToString() const
{
	return m_cast[0] + ",  " + m_cast[1] + ",  " + m_cast[2] + ",  " + m_cast[3] + ",  " + m_cast[4];
}

unsigned int Film::getReleaseYear() const
{
	return m_releaseYear;
}

const std::string& Film::getDirector() const
{
	return m_director;
}

const std::string& Film::getTitle() const
{
	return m_title;
}

std::unordered_set<enum class Genre> Film::getGenres() const
{
	return m_genres;
}

void Film::setHighlighted(bool highlighted)
{
	m_highlighted = highlighted;
}

bool Film::isHighlighted() const
{
	return m_highlighted;
}

void Film::setActive(bool active)
{
	m_active = active;
	if (active) {
		s_filmState = FilmState::FILM_CLICKED;
	}
	else {
		s_filmState = FilmState::NO_FILM_CLICKED;
	}
}
bool Film::isActive() const
{
	return m_active;
}

const std::vector<std::string> Film::tokenizeDescription() const
{
	std::vector<std::string> words;				// "words" will be the vector that contains all the words from our description, split every ' '
	std::stringstream tokenizer(m_description);
	std::string word;							//  "word" will represent each unique word from our description

	while (std::getline(tokenizer, word, ' ')) {
		words.push_back(word);
	}

	// As of now, words has all the unique words.

	/* Initialising a new vector of strings (called "lines") that will contain all the unique lines to be
	*  returned. We will be simultaneously iterating through both the vector that includes all our current
	*  words, and the new "lines" vector. If the currently iterated on "lines" string reaches a custom
	*  threshold (100 in our case), we push back a new empty string, and start working with that one.*/

	std::vector<std::string> lines;
	lines.push_back("");
	int i{ 0 };									// using a second "iterator" to simultaneously iterate through words and lines 
	for (int j{ 0 }; j < words.size(); ++j) {
		// if appending the current word would take us over our custom threshold, pushing back a new empty string and working on that one.
		if (lines[i].size() + words[j].size() + 1 >= 100) {
			lines.push_back("");
			++i;
		}
		lines[i].append(words[j]).append(" ");
	}
	return lines;
}