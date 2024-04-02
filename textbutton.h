#pragma once
#include "button.h"
#include <string>

/* A text button will have just what the name indicates; only a string of text. During this project's creation, we've ended up
 * with 3 types of Text Buttons: an "Advanced Search" button (used in the main menu screen), an "Apply Filters" button (used in
 * the search screen), and a "Clear Filters" button (also used in the search screen). So, we will create an enum class of the 3
 * aforementioned types, along with an "Other" type as a shallow attempt at futureproof code, and in case of inherited classes 
 * (see: GenreButton).	*/

enum class TextButtonUseage { ADVANCED_SEARCH, APPLY_FILTERS, CLEAR_FILTERS, OTHER };

// making TextButton virtual public, because we might construct a class that derives from both TextButton and TextureButton
class TextButton : virtual public Button {
private:
	// m_useage: the variable used to store a TextButton's function. Private, as our subclasses have no purpose knowing this
	const TextButtonUseage m_useage;

protected:
	std::string m_text;			// The text a TextButton will contain.

public:
	TextButton(const float pos_x, const float pos_y, const float width, const float height, const std::string& text, const TextButtonUseage useage);
	virtual ~TextButton();

	virtual void draw() override;
};