#pragma once
#include "widget.h"
#include "texturebutton.h"
#include <string>
#include <algorithm>	// for std::transform

 /* A textfield will be used for one of 4 purposes: to filter an actor's name, a director's name, a film title or all of the
 * above, from the films passed to it to filter. Therefore, making a custom enum regarding a textfield's useage, with those 4 values. */
enum class TextfieldUseage {ACTOR, DIRECTOR, GENERAL, TITLE};

class TextField : public Widget {
private:
	std::string m_string;				// The text every Textbutton will display.
	class TextureButton m_button;		// each TextField will have a small "x" button to its right, that if clicked, will reset its state.

	// Textfields will have a width and height variable, since they are rectangular:
	float m_width;
	float m_height;

	const TextfieldUseage m_useage;		// m_useage: Represents the useage of a textfield.

	/* offsetString: Implementing a custom condition based on the size of the text we're printing:
	 * If the size of the string is smaller than width / 12.0f, draw the whole string. Otherwise,
	 * draw the last (width / 12.0f) characters of the string. To prevent text escaping our bounds. */
	const std::string offsetString() const;

public:
	TextField(const float pos_x, const float pos_y, const float width, const float height, const TextfieldUseage useage);

	void draw() override;
	void update() override;
	void resetState() override;

	// a textfield is rectangular, therefore its contains will utilise our "rectangularContains" helper function.
	virtual bool contains(const float x, const float y) const override;

	bool canFilter() const override;
	virtual void filter(std::unordered_set<class Film*>& currFilms) const override;
};