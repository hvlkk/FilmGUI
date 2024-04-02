#pragma once
#include "button.h"

/* Similar to our Text Button, a Texture Button will be used for buttons that have nothing but an image associated with them.
 * A texture button will by default be drawn at max fill opacity, to a transparent background. So far, we've used an undo
 * button (which we'll be filing under "DEFAULT"), an x button for each textfield, and we're again using an "other" type as a 
 * shallow attempt at futureproof code, and in case of inherited classes. */

enum class TextureButtonUseage {DEFAULT, TEXTFIELD_X, OTHER};

// making TextureButton virtual public, because we might construct a class that derives from both TextButton and TextureButton
class TextureButton : virtual public Button {
protected:
	std::string m_texture;					// The texture a TextureButton will contain. Passed as a fileName on initalisation.
	const TextureButtonUseage m_useage;		// For each button's unique useage.


public:
	TextureButton(const float pos_x, const float pos_y, const float width, const float height, const std::string& fileName, const TextureButtonUseage useage);
	virtual ~TextureButton();

	virtual void draw() override;

	virtual void update() override;
};