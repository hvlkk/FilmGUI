#include "genrebutton.h"

#include <utility>

// calculates the width based on the name of the genre (passed as param)
float GenreButton::calculateWidth(Genre genre)
{
	auto size = genreName(genre).size();
	return size * 10.2f;
}

GenreButton::GenreButton(const float pos_x, const float pos_y, const float height, const Genre genre) :
	TextButton{pos_x, pos_y, calculateWidth(genre), height, genreName(genre), TextButtonUseage::OTHER},
	Button{pos_x, pos_y, calculateWidth(genre), height},
	m_genre{genre}
{
}

void GenreButton::draw()
{
	// drawing the rectangle of the button
	float highlighted{ 0.3f + m_highlighted * 0.4f };

	m_brush.outline_opacity = 1.0f;
	SETCOLOUR(m_brush.outline_color, highlighted, highlighted, highlighted);

	// printing the button in a slightly darker colour if it has been clicked
	if (m_clicked) {
		SETCOLOUR(m_brush.fill_color, 1.0f, 0.9f, 0.2f);
	}
	else {
		SETCOLOUR(m_brush.fill_color, 0.6f, 0.5f, 0.0f);
	}
	graphics::drawRect(m_pos_x, m_pos_y, m_width, m_height, m_brush);

	SETCOLOUR(m_brush.fill_color, 0.15f, 0.15f, 0.15f);
	m_brush.outline_opacity = 0.0f;
	graphics::drawText(m_pos_x-m_width / 2.175f, m_pos_y + m_height / 4.0f, m_height * 0.625f, genreName(m_genre), m_brush);

}

// A Genre Button is valid for filtering if it has been clicked (m_clicked == true)
bool GenreButton::canFilter() const
{
	return m_clicked;
}

// if the genre button is clicked, it filters the unordered_set of films passed to it, removing any that don't have its genre
void GenreButton::filter(std::unordered_set<class Film*>& currFilms) const
{
	// iterating through the unordered_set of films
	auto iter { currFilms.begin() };
	while (iter != currFilms.end()) {

		// fetching the respective unordered_set of genres for every film, and iterating through it
		std::unordered_set<enum class Genre> genres { (*iter)->getGenres() };
		auto genre_iter { genres.begin() };
		while (genre_iter != genres.end()) {

			// if we have found the genre this Genre Button contains, breaking the loop and continuing with the next film in the set
			if (*genre_iter == m_genre) {
				break;
			}
			++genre_iter;
		}

		// if we went through all the genres but didn't find the one this Genre Button has, erasing the film from the set, while incrementing the iterator
		if (genre_iter == genres.end()) {
			currFilms.erase(iter++);
			continue;
		}
		++iter;
	}
}

/*
void GenreButton::update()
{
}
*/