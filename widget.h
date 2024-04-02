#pragma once
#include "global.h"
#include "util.h"
#include "film.h"
#include "graphics.h"

#include <unordered_set>

/* Widget: The parent class of all widgets in our film browser. Even though all the widgets in our project are rectangular so far,
 * for futureproof reasons we will not be including width and height here because we might make a non-rectangular widget at some
 * point in the future. */

class Widget {
protected:
	static inline unsigned int s_widgetID{ 0 };		// C++17 - directly initialising a static variable inside our class
	unsigned int m_widgetID;						// m_widgetID: so that every widget has a unique ID
	
	struct graphics::Brush m_brush;
	
	float m_pos_x;
	float m_pos_y;

	float m_init_pos_x;
	float m_init_pos_y;

	// m_highlighted: A widget is highlighted when our mouse is hovering over it
	float m_highlighted;

	// m_clicked: Used so each widget knows if it has been clicked or not
	bool m_clicked;


public:
	Widget(const float pos_x, const float pos_y) :
		m_pos_x { pos_x },
		m_pos_y { pos_y },
		m_highlighted { false },
		m_widgetID { s_widgetID++ },
		m_init_pos_x { pos_x },
		m_init_pos_y { pos_y },
		m_clicked { false }
	{}

	// virtual destructor, in case any child classes have dynamically allocated memory that they will need to free up.
	virtual ~Widget()												{}

	// Set/Get PosX/PosY: Standard getters/setters.

	float getPosX() const									{ return m_pos_x; }
	float getPosY() const									{ return m_pos_y; }
	void setPosX(float pos_x)								{ m_pos_x = pos_x; }
	void setPosY(float pos_y)								{ m_pos_y = pos_y; }

	// for contains: args mouse_x, mouse_y; to check if our mouse is hovering over the widget.
	// Each widget will have their own implementation, based on whether it is rectangular, circular or anything else.
	virtual bool contains(const float x, const float y) const = 0;	
	
	/* canFilter: Used to check if a widget (in its current state) can filter films. Used when applying filters, to see
	 * which widgets should be taken into consideration for filtering. Returns false by default, the widgets that should
	 * be allowed to filter have their own implementations, overriding this one. */
	virtual bool canFilter() const							{ return false; }

	/* Called when filtering the films, parameter is the unordered_set of films to be filtered. If the widget is currently 
	 * filtering (canFilter()==true), it erases all films that do not fit its criteria from the unordered_set parameter. */
	virtual void filter(std::unordered_set<class Film*>& currFilms) const { if (!canFilter()) return; }

	// Returns whether a widget has been clicked on.
	bool isClicked() const { return m_clicked; }
	
	/* Resets the state of the widget to the original state. Specifically, if the widget has been clicked it resets its
	 * clicked state to false, and if it has been moved, it resets its coordinates to its original coordinates. */
	virtual void resetState() {
		m_clicked = false;
		if (m_pos_x != m_init_pos_x) m_pos_x = m_init_pos_x;
		if (m_pos_y != m_init_pos_y) m_pos_y = m_init_pos_y;
	}

	virtual void draw() = 0;
	virtual void update() = 0;
};