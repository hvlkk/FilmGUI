#pragma once
#include "textbutton.h"
#include "filmutil.h"

#include <utility>

// GenreButton is a self-contained class we don't want to allow inheritance from, therefore making it final.
class GenreButton final : public TextButton {
private:
	Genre m_genre;

public:
	// the GenreButton constructor does not take width as a parameter, because width
	// is automatically generated on creation based on the (size of the) name of the genre
	GenreButton(const float pos_x, const float pos_y, const float height, const Genre genre);

	void draw() override;
	// void update() override;	- DEPRECATED: GenreButton will utilise Button's update()
	bool canFilter() const override;
	void filter(std::unordered_set<class Film*>& currFilms) const override;

	// calculates the width based on the name of the genre (passed as param)
	static float calculateWidth(Genre genre);	// static, since we'll be using it in FilmUI to calculate the width of a GenreButton before initialising it
};