#pragma once
#include "widget.h"
#include "textbutton.h"
#include "texturebutton.h"
#include "slider.h"
#include "genrebutton.h"
#include "textfield.h"

#include "film.h"
#include "filmutil.h"
#include "global.h"
#include "util.h"
#include "graphics.h"

#include <string>
#include <vector>
#include <unordered_set>

// FilmUI will be our main class. It will be the one responsible for all the films and widgets.

class FilmUI {
private:

	// Using an enum class to make switching between the 3 states of the UI easy.
	enum class FilmUIState { MAIN_MENU, SEARCH_SCREEN, SEARCH_RESULTS };

	// Our class will be a singleton; using the following static variable to serve as the only instance
	static inline FilmUI* m_instance{nullptr};						// C++17 - initialising our static instance inside the class
	FilmUIState m_state = FilmUIState::MAIN_MENU;

	// m_minYear, m_maxYear: Represent the minimum/maximum available release year our database contains. Calculated on instantiation.
	unsigned int m_minYear{ 10000 };
	unsigned int m_maxYear {0};

	// m_films: All the films in our project
	std::unordered_set<class Film*> m_films;

	/* m_currentFilms: For the search results. Initially contains all the films in our project, but every "valid" widget
	 * that accesses it (every widget that can filter at a set point of time), modifies its elements and removes the ones
	 * that don't fit its filter. */
	std::unordered_set<Film*> m_currentFilms;

	// m_widgets: for all the widgets included in our project
	std::vector<class Widget*> m_widgets;

	// m_currentWidgets: for the widgets in the current screen ("state")
	std::vector<Widget*> m_currentWidgets;

	// m_brush: FilmUI will have its own Brush member variable for drawing purposes
	struct graphics::Brush m_brush;
	
	// m_activeFilm: Signifies the film we've clicked on
	Film* m_activeFilm{ nullptr };

	// called after every iteration of draw, resets our Brush member variable
	void resetBrush();

	// Called on initialisation, fetches the minimum and maximum release year from our films
	void calculateYearBounds();

	// Used during updates in the MAIN_MENU and SEARCH_RESULTS states. Handles the films and the way they update.
	void filmUpdate();
	
	// Update/Draw Main Menu, Search Screen, Search Results: The respective draw/update functions we call based on the state of the UI.
	
	void updateMainMenu();
	void updateSearchScreen();
	void updateSearchResults();
	void drawMainMenu();
	void drawSearchScreen();
	void drawSearchResults();

	// Called when we go back from the search screen to the main menu
	void returnToMainMenu();

	// Called when we go from the main menu to the search screen
	void initialiseSearchScreen();

	// Called when we go from the search screen to the search results
	void initialiseSearchResults();

	// Lengthy initialisation of all films, done outside init() for readability purposes
	void initialiseFilms();

	// Lengthy initialisation of all widgets, done outside init() for readability purposes
	void initialiseWidgets();

	FilmUI() = default;

public:
	void update();
	void draw();
	void init();

	// To get the currently active instance of FilmUI
	static FilmUI* getInstance();

	// To release the currently active instance of FilmUI
	static void releaseInstance();

	~FilmUI();
};