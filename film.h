#pragma once
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

/* Using a custom enum to signify 2 states in regards to the films being drawn:
*  If we haven't yet clicked on any film, any film that we're hovering over will have its relevant info shown at the bottom of the screen.
*  If we have clicked on one, only that film will have its info shown at the bottom, regardless of which one we're hovering over.
*  
*  **workaround so we don't have to include a FilmUI* in every Film instance**
*/
enum class FilmState {FILM_CLICKED, NO_FILM_CLICKED};

class Film {
private:
	/* using a unique ID for each film, to both have something to uniquely identify them by, and to
    *  help us pass coordinates in a semi-automated way on initialisation (showcased in the constructor) */
	static inline unsigned int s_filmID{0};
	unsigned int m_filmID;

	// making the enum var static so whenever we click on a film, all the other films know
	static inline FilmState s_filmState{ FilmState::NO_FILM_CLICKED };

	const float m_posterWidth{ 105 };					// the width of a film's poster

	const float m_posterHeight{ 160 };					// the height of a film's poster

	const std::string m_poster;							// the string of the png of the film poster

	const std::string m_title;
	const unsigned int m_releaseYear;
	const std::string m_director;
	std::unordered_set<enum class Genre> m_genres;
	std::string m_description;
	std::array<std::string, 5> m_cast;					// using an array of size 5 to have a common amount of actors per film

	struct graphics::Brush m_brush;						// each film will have its own Brush member variable
	float m_pos_x;
	float m_pos_y;

	// tokenizeDescription: helper function to tokenize a film's description for accurate drawing
	const std::vector<std::string> tokenizeDescription() const;

	bool m_highlighted { false };						// a film is highlighted when our mouse is hovering over it
	bool m_active { false };							// a film is "active" if we have clicked on it - makes all other films unavailable for highlighting

public:
	Film(const std::string_view title, unsigned int releaseYear, const std::string_view director,
		const std::unordered_set<Genre>& genres, const std::array<std::string, 5>& cast,
		const std::string_view description, const std::string_view poster);
	
	~Film();

	void draw();

	// drawLineOfText: extra function so that we don't clutter our draw function. 5 parameters, first 2 are the canvas width/height
	// offset, and the rest of the parameters are the ones graphics::drawText needs (excluding Brush since it will be our member variable)
	void drawLineOfText(float offsetX, float offsetY, float size, const std::string& attribute, const std::string& text);

	// DEPRECATED: Film's update is not used in this project.
	void update();

	// DEPRECATED: setPosX & setPosY are not used in this project.
	void setPosX(const float pos_x);
	void setPosY(const float pos_y);

	// contains: to check if our mouse coordinates are inside a film poster
	bool contains(const float x, const float y) const;

	// setHighlighted: Setter for film's "m_highlighted" variable.
	void setHighlighted(bool highlighted);

	// isHighlighted: Returns true if we're hovering over a film's poster.
	bool isHighlighted() const;

	// setActive: Setter for film's "m_active" variable. 
	void setActive(bool active);

	// isActive: Returns true if we've clicked on a film's poster.
	bool isActive() const;

	/* getReleaseYear/getDirector/getTitle/getGenres: Typical getters. */

	unsigned int getReleaseYear() const;
	const std::string& getDirector() const;
	const std::string& getTitle() const;
	std::unordered_set<enum class Genre> getGenres() const;

	// castToString: Return a film's cast as a single string. Used when drawing the film cast on the screen.
	std::string castToString() const;
};