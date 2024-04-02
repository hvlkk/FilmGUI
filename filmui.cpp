#include "filmui.h"
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

// called upon initialisation of the instance
void FilmUI::init()
{
	graphics::preloadBitmaps(ASSET_PATH);
	initialiseFilms();
	calculateYearBounds();
	initialiseWidgets();
	setFont("Montserrat-Medium.ttf");
}

// called whenever we go from main menu to search screen
void FilmUI::initialiseSearchScreen()
{
	resetBrush();
	m_state = FilmUIState::SEARCH_SCREEN;
	m_currentWidgets.clear();
	
	/* The way filtering works: m_current_films serves as a temporary container that will have all the films in our database.
	 * When we apply any filter (e.g. by clicking on a genre button), that widget is responsible of handling the entire
	 * m_current_films container, and removing any films that do not fit its filter.*/
	m_currentFilms = m_films;

	// pushing back the undo button, the 2 sliders, the 10 genre buttons and the 3 textfields
	for (unsigned int i{ 2 }; i < 20; ++i) {
		m_currentWidgets.push_back(m_widgets[i]);
	}
}

void FilmUI::initialiseSearchResults()
{
	resetBrush();
	m_state = FilmUIState::SEARCH_RESULTS;
	for (auto& widget : m_currentWidgets) {
		if (widget->canFilter()) {
			widget->filter(m_currentFilms);
		}
	}
	m_currentWidgets.clear();
	m_currentWidgets.push_back(m_widgets[2]);
}

// called when we click "close" on the search screen
void FilmUI::returnToMainMenu()
{
	resetBrush();
	m_state = FilmUIState::MAIN_MENU;
	for (auto& widget : m_currentWidgets) {
		widget->resetState();
	}
	m_currentWidgets.clear();
	m_currentWidgets.push_back(m_widgets[0]);		// pushing back the main menu textfield
	m_currentWidgets.push_back(m_widgets[1]);		// pushing back the "advanced search" button
}

void FilmUI::draw()
{
	if (m_state == FilmUIState::MAIN_MENU) {
		drawMainMenu();
	}
	else if (m_state == FilmUIState::SEARCH_SCREEN) {
		drawSearchScreen();
	}
	else {
		drawSearchResults();
	}
}

void FilmUI::update()
{
	if (m_state == FilmUIState::MAIN_MENU) {
		updateMainMenu();
	}
	else if (m_state == FilmUIState::SEARCH_SCREEN) {
		updateSearchScreen();
	}
	else {
		updateSearchResults();
	}
}

FilmUI::~FilmUI()
{
	for (auto& film : m_films) {
		delete film;
	}
	for (auto& widget : m_widgets) {
		delete widget;
	}
	m_films.clear();
	m_widgets.clear();
	m_currentFilms.clear();
	m_currentWidgets.clear();
}

// used when in the main menu state
void FilmUI::updateMainMenu()
{

	// if the textfield has filtered the current films, re-initialise them so that textfield filters them anew
	if (m_currentFilms != m_films) {
		m_currentFilms = m_films;
	}

	// printing the current widgets
	for (auto& widget : m_currentWidgets) {
		widget->update();

		// if we have clicked on the textfield and we have entered some text to it:
		if (widget == m_widgets[0] && widget->canFilter()) {
			widget->filter(m_currentFilms);
		}

		if (widget == m_widgets[1] && widget->isClicked()) {
			widget->resetState();
			initialiseSearchScreen();
			break;
		}
	}

	filmUpdate();
}

void FilmUI::updateSearchScreen()
{
	graphics::MouseState mouse;
	graphics::getMouseState(mouse);

	float mouse_x{ graphics::windowToCanvasX(static_cast<float>(mouse.cur_pos_x)) };
	float mouse_y{ graphics::windowToCanvasY(static_cast<float>(mouse.cur_pos_y)) };

	for (auto& widget : m_currentWidgets) {
		widget->update();
		// if we click on the undo button, reset its state, and go back to MAIN_MENU state
		if (widget == m_widgets[2] && widget->isClicked()) {
			widget->resetState();
			returnToMainMenu();
			break;
		}

		// if we click on "Apply Filters", move to the SEARCH_RESULTS state
		if (widget == m_widgets[15] && widget->isClicked()) {
			widget->resetState();
			initialiseSearchResults();
			break;
		}

		// if we click on "Clear Filters", reset the state of all gadgets, and reset the unordered_set of current films
		if (widget == m_widgets[16] && widget->isClicked()) {
			for (auto& currWidget : m_currentWidgets) {
				currWidget->resetState();
			}
			m_currentFilms = m_films;
		}
	}
}

void FilmUI::updateSearchResults()
{
	// same thing as in updateMainMenu() in regards to films
	filmUpdate();

	for (auto& widget : m_currentWidgets) {
		widget->update();
		// if we click on the undo button
		if (widget == m_widgets[2] && widget->isClicked()) {
			widget->resetState();
			initialiseSearchScreen();
			break;
		}
	}
}

// the first "state" of our app, when we've just launched the app, or when we've clicked the close button on the advanced search screen
void FilmUI::drawMainMenu()
{
	// drawing the background 
	setTexture(m_brush, "background.png");
	m_brush.outline_opacity = 0.0f;
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, m_brush);

	// drawing all our films, 2 rows of 5 columns
	for (auto& film : m_currentFilms) {
		film->draw();
	}

	for (auto& widget : m_currentWidgets) {
		widget->draw();
	}

	// drawing a background for the film info
	SETCOLOUR(m_brush.fill_color, 0.1f, 0.1f, 0.1f);
	m_brush.fill_opacity = 0.9f;
	setTexture(m_brush, "");
	graphics::drawRect(canvasWidthOffset(0.5f), canvasHeightOffset(0.9f), CANVAS_WIDTH, canvasHeightOffset(0.43f), m_brush);

	resetBrush();
	
}

// the second "state" of our app, when we've clicked on "Advanced Search" on the main menu (or clicked on the back button on the results screen)
void FilmUI::drawSearchScreen()
{
	// drawing the background,
	setTexture(m_brush, "graphite2.png");
	SETCOLOUR(m_brush.fill_color, 1.0f, 1.0f, 1.0f);
	graphics::drawRect(canvasWidthOffset(0.5f), canvasHeightOffset(0.5f), CANVAS_WIDTH, CANVAS_HEIGHT, m_brush);

	// inside which will be a rectangle centered towards the middle, in which we will have the sliders to filter the year, the genre
	// buttons, the textfields for title/"person" (aka director + actor filter), an "apply filters" button, and a "reset filters" button

	setTexture(m_brush, "");
	SETCOLOUR(m_brush.fill_color, 0.05f, 0.05f, 0.05f);
	m_brush.outline_opacity = 0.25f;
	SETCOLOUR(m_brush.outline_color, 0.5f, 0.5f, 0.5f);
	m_brush.fill_opacity = 0.9f;
	graphics::drawRect(canvasWidthOffset(0.5f), canvasHeightOffset(0.5f), canvasWidthOffset(0.66f), canvasHeightOffset(0.66f), m_brush);

	// "Genres", top left
	setFont("Montserrat-Medium.ttf");
	SETCOLOUR(m_brush.fill_color, 1.0f, 1.0f, 1.0f);
	m_brush.fill_opacity = 1.0f;
	m_brush.outline_opacity = 0.0f;
	graphics::drawText(canvasWidthOffset(0.18f), canvasHeightOffset(0.2f), 18, "Genres:", m_brush);

	// "Year", middle left
	graphics::drawText(canvasWidthOffset(0.18f), canvasHeightOffset(0.575f), 18, "Year:", m_brush);

	for (auto& widget : m_currentWidgets) {
		widget->draw();
	}

	resetBrush();
}

// the third "state" of our app, when we've applied some filters and get the results back
void FilmUI::drawSearchResults()
{

	// drawing the background 
	setTexture(m_brush, "background.png");
	m_brush.outline_opacity = 0.0f;
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, m_brush);

	// drawing all our films, 2 rows of 5 columns
	for (auto& film : m_currentFilms) {
		film->draw();
	}

	for (auto& widget : m_currentWidgets) {
		widget->draw();
	}

	// drawing a background for the film info
	SETCOLOUR(m_brush.fill_color, 0.1f, 0.1f, 0.1f);
	m_brush.fill_opacity = 0.9f;
	setTexture(m_brush, "");
	graphics::drawRect(canvasWidthOffset(0.5f), canvasHeightOffset(0.9f), CANVAS_WIDTH, canvasHeightOffset(0.43f), m_brush);

	// if our search failed, drawing "No results found" on the screen.
	if (m_currentFilms.size() == 0) {
		SETCOLOUR(m_brush.fill_color, 0.2f, 0.2f, 0.2f);
		graphics::drawText(canvasWidthOffset(0.45f), canvasHeightOffset(0.5f), 30, "No Results Found.", m_brush);
	}
	// else, for each film that was returned, print their respective info.
	else {
		for (auto& film : m_currentFilms) {
			film->draw();
		}
	}
	resetBrush();
}

// resets FilmUI's brush member variable
void FilmUI::resetBrush()
{
	setTexture(m_brush, "");
	SETCOLOUR(m_brush.fill_color, 1.0f, 1.0f, 1.0f);
	SETCOLOUR(m_brush.outline_color, 1.0f, 1.0f, 1.0f);
	m_brush.fill_opacity = 1.0f;
	m_brush.outline_opacity = 0.0f;		// more items will not have an outline rather than will, therefore making the default value 0.0f
}

// called on initialisation, fetches the minimum and maximum release year from our films
void FilmUI::calculateYearBounds()
{
	// minYear is initialised at 10000, maxYear is initialised at 0

	for (auto& film : m_films) {
		if (film->getReleaseYear() < m_minYear) {
			m_minYear = film->getReleaseYear();
		}
		if (film->getReleaseYear() > m_maxYear) {
			m_maxYear = film->getReleaseYear();
		}
	}
}

// Used during updates in the MAIN_MENU and SEARCH_RESULTS states. Handles the films and the way they update.
void FilmUI::filmUpdate()
{
	graphics::MouseState mouse;
	graphics::getMouseState(mouse);
	float mouse_x{ graphics::windowToCanvasX(static_cast<float>(mouse.cur_pos_x)) };
	float mouse_y{ graphics::windowToCanvasY(static_cast<float>(mouse.cur_pos_y)) };

	for (auto& film : m_currentFilms) {
		film->update();
		film->setHighlighted(film->contains(mouse_x, mouse_y));

		/* 2 states in regards to how films will be outlined; if no film has been clicked (and m_activeFilm == nullptr)
		 *  any film we hover on will have its info displayed at the bottom of the screen. If a film has been clicked
		 *  (so m_activeFilm!=nullptr), only the highlighted film will have its info shown at the bottom of the screen */
		if (!m_activeFilm) {
			if (film->isHighlighted() && mouse.button_left_pressed) {
				m_activeFilm = film;
				film->setActive(true);
			}
		}
		else {
			// if we have already selected a film and we click anywhere else on the screen, we go back to the previous state
			if (m_activeFilm == film && !(film->contains(mouse_x, mouse_y)) && mouse.button_left_pressed) {
				m_activeFilm = nullptr;
				film->setActive(false);

				// if the coordinates we've clicked on belong to any  other film, make that film active
				for (auto& otherFilm : m_films) {
					if (otherFilm->contains(mouse_x, mouse_y)) {
						m_activeFilm = otherFilm;
						otherFilm->setActive(true);
					}
				}
			}
		}
	}
}

FilmUI* FilmUI::getInstance()
{
	if (!m_instance) {
		m_instance = new FilmUI();
	}
	return m_instance;
}

void FilmUI::releaseInstance()
{
	if (m_instance) {
		delete m_instance;
	}
	m_instance = nullptr;
}

// lengthy initialisation, initialises all widgets
void FilmUI::initialiseWidgets()
{
	// m_widgets represents all the widgets in our application. m_currentWidgets represents the widgets in our current app
	// state (MAIN_MENU, SEARCH_SCREEN and SEARCH_RESULTS respectively). Initialised with the main menu widgets.

	// reserving 25 spots for both m_widgets and m_currentWidgets, since we have 19 widgets and resizing is relatively expensive
	m_widgets.reserve(25);
	m_currentWidgets.reserve(25);

	// main menu textfield - widget[0]
	TextField* mainMenuTextfield{ new TextField{canvasWidthOffset(0.75f), canvasHeightOffset(0.04f), 220, 30, TextfieldUseage::GENERAL}};
	m_widgets.push_back(mainMenuTextfield);
	m_currentWidgets.push_back(mainMenuTextfield);

	// advanced search button - widgets[1]
	// in main menu widgets, not in search screen widgets, in search result widgets
	TextButton* adv{ new TextButton{canvasWidthOffset(0.906f), canvasHeightOffset(0.047f), 140, 25, "Advanced Search", TextButtonUseage::ADVANCED_SEARCH}};
	m_widgets.push_back(adv);
	m_currentWidgets.push_back(adv);

	// undo button - widgets[2]
	// not in main menu widgets, in search screen widgets, in search result widgets
	TextureButton* undo{ new TextureButton{canvasWidthOffset(0.96f), canvasHeightOffset(0.05f), 30, 30, "undo.png", TextureButtonUseage::DEFAULT} };
	m_widgets.push_back(undo);
	
	// slider1 & slider2 - widgets[3] & widgets[4]
	// not in main menu widgets, in search screen widgets, not in search result widgets
	Slider* slider1{ new Slider{canvasWidthOffset(0.375f), canvasHeightOffset(0.675f), 18, 42, m_minYear, m_maxYear, SliderUseage::LOWER_BOUND} };
	Slider* slider2{ new Slider{canvasWidthOffset(0.375f), canvasHeightOffset(0.75f), 18, 42, m_minYear, m_maxYear, SliderUseage::UPPER_BOUND} };
	m_widgets.push_back(slider1);
	m_widgets.push_back(slider2);


	// the 10 genre buttons - widgets[5]-widgets[14]
	// not in main menu widgets, in search screen widgets, not in search result widgets

	// previousUpperBound: Using a custom variable to mark the previous GenreButton's rightmost x coordinate.
	// All GenreButtons will have to be in the search screen's inner rectangle, therefore starting drawing them from canvasWidthOffset(0.18f)
	float previousUpperBound { canvasWidthOffset(0.18f) };

	// when we reach a certain position of the canvas width, we will be drawing the following GenreButtons
	// slightly lower on the canvas (incrementing currentHeightOffset by 0.06f)
	float currentHeightOffset { canvasHeightOffset(0.25f) };

	for (const auto genre : genresToArray()) {

		// calculating the width the current GenreButton will require
		float currentWidth{ GenreButton::calculateWidth(genre) };

		// if drawing a GenreButton will not take us over our custom limit (canvasWidthOffset(0.5f)
		if (previousUpperBound + currentWidth < canvasWidthOffset(0.5f)) {
			GenreButton* genreButton{ new GenreButton{previousUpperBound + 10.0f + currentWidth * 0.5f, currentHeightOffset, 22.5f, genre} };
			m_widgets.push_back(genreButton);
			previousUpperBound = genreButton->getPosX() + genreButton->getWidth() / 2.0f;
		}
		// else, if drawing the GenreButton would take us over our custom limit, resetting previousUpperBound, incrementing currHeightOffset
		else {
			previousUpperBound = canvasWidthOffset(0.18f);
			currentHeightOffset += canvasHeightOffset(0.06f);
			GenreButton* genreButton{ new GenreButton{previousUpperBound + 10.0f + currentWidth * 0.5f, currentHeightOffset, 22.5f, genre} };
			m_widgets.push_back(genreButton);
			previousUpperBound = genreButton->getPosX() + genreButton->getWidth() / 2.0f;
		}
	}

	// apply filters button - widgets[15]
	// not in main menu widgets, in search screen widgets, not in search result widgets

	TextButton* apply{ new TextButton{canvasWidthOffset(0.7f), canvasHeightOffset(0.3f), 100, 25, "Apply Filters", TextButtonUseage::APPLY_FILTERS} };
	m_widgets.push_back(apply);

	// clear filters button - widgets[16]
	// not in main menu widgets, in search screen widgets, not in search result widgets

	TextButton* clear{ new TextButton{canvasWidthOffset(0.7f), canvasHeightOffset(0.36f), 100, 25, "Clear Filters", TextButtonUseage::CLEAR_FILTERS} };
	m_widgets.push_back(clear);

	// the three textfields (actor, director and title textfields) - widgets[17]-[19]
	// not in main menu widgets, in search screen widgets, not in search result widgets

	TextField* actorTextfield{ new TextField{canvasWidthOffset(0.7f), canvasHeightOffset(0.65f), 220, 30, TextfieldUseage::ACTOR} };
	m_widgets.push_back(actorTextfield);

	TextField* directorTextField{ new TextField(canvasWidthOffset(0.7f), canvasHeightOffset(0.71f), 220, 30, TextfieldUseage::DIRECTOR) };
	m_widgets.push_back(directorTextField);

	TextField* titleTextField{ new TextField{canvasWidthOffset(0.7f), canvasHeightOffset(0.77f), 220, 30, TextfieldUseage::TITLE} };
	m_widgets.push_back(titleTextField);
}

// very lengthy initialisation, initialises 10 films with their respective titles and further info, feel free to ignore
void FilmUI::initialiseFilms()
{
	std::string_view title{ "12 Angry Men" };
	std::string_view director{ "Sidney Lumet" };
	unsigned int releaseYear{ 1957 };
	std::array<std::string, 5> cast;
	cast[0] = "Henry Fonda";
	cast[1] = "Lee J. Cobb";
	cast[2] = "John Fiedler";
	cast[3] = "Martin Balsalm";
	cast[4] = "E.G.Marshall";
	std::unordered_set<Genre> genres;
	genres.insert(Genre::drama);
	std::string_view description{"The defense and the prosecution have rested and the jury is filing into the jury room to decide if a young Spanish-American is guilty or innocent of murdering his father. What begins as an open and shut case soon becomes a mini-drama of each of the jurors' prejudices and preconceptions about the trial, the accused, and each other."};
	std::string_view poster{ "12AngryMen.png" };
	Film* film{ new Film{title, releaseYear, director, genres, cast, description, poster} };
	m_films.insert(film);
	genres.clear();

	title = "Chungking Express";
	director = "Wong Kar-Wai";
	releaseYear = 1994;
	cast[0] = "Brigitte Lin";
	cast[1] = "Takeshi Kaneshiro";
	cast[2] = "Tony Leung Chiu-Wai";
	cast[3] = "Faye Wong";
	cast[4] = "Valerie Chow";
	genres.insert(Genre::comedy);
	genres.insert(Genre::crime);
	genres.insert(Genre::drama);
	genres.insert(Genre::romance);
	description = "Two melancholic Hong Kong policemen fall in love: one with a mysterious underworld figure, the other with a beautiful and ethereal server at a late-night restaurant he frequents.";
	poster = "ChungkingExpress.png";
	film = new Film{ title, releaseYear, director, genres, cast, description, poster };
	m_films.insert(film);
	genres.clear();

	title = "Eternal Sunshine of the Spotless Mind";
	director = "Michel Gondry";
	releaseYear = 2004;
	cast[0] = "Jim Carrey";
	cast[1] = "Kate Winslet";
	cast[2] = "Kirsten Dunst";
	cast[3] = "Mark Ruffalo";
	cast[4] = "Elijah Wood";
	genres.insert(Genre::drama);
	genres.insert(Genre::romance);
	description = "Joel Barish, heartbroken that his girlfriend underwent a procedure to erase him from her memory, decides to do the same. However, as he watches his memories of her fade away, he realises that he still loves her, and may be too late to correct his mistake.";
	poster = "EternalSunshine.png";
	film = new Film{ title, releaseYear, director, genres, cast, description, poster };
	m_films.insert(film);
	genres.clear();

	title = "Grave of the Fireflies";
	director = "Isao Takahata";
	releaseYear = 1988;
	cast[0] = "Tsutomu Tatsumi";
	cast[1] = "Ayano Siraishi";
	cast[2] = "Yoshiko Shinohara";
	cast[3] = "Akemi Yamaguchi";
	cast[4] = "Masayo Sakai";
	genres.insert(Genre::animation);
	genres.insert(Genre::drama);
	genres.insert(Genre::war);
	description = "In the final months of World War II, 14-year-old Seita and his sister Setsuko are orphaned when their mother is killed during an air raid in Kobe, Japan. After a falling out with their aunt, they move into an abandoned bomb shelter. With no surviving relatives and their emergency rations depleted, Seita and Setsuko struggle to survive.";
	poster = "GraveOfTheFireflies.png";
	film = new Film{ title, releaseYear, director, genres, cast, description, poster };
	m_films.insert(film);
	genres.clear();

	title = "Memories of Murder";
	director = "Bong Joon-ho";
	releaseYear = 2003;
	cast[0] = "Song Kang-ho";
	cast[1] = "Kim Sang-kyung";
	cast[2] = "Park No-shik";
	cast[3] = "Byun Hee-bong";
	cast[4] = "Kim Roe-ha";
	genres.insert(Genre::mystery);
	genres.insert(Genre::thriller);
	description = "1986, Gyunggi Province. The body of a young woman is found brutally raped and murdered. Two months later, a series of rapes and murders commences under similar circumstances. A special task force is set up in the area, with two local detectives joined by a detective from Seoul who requested to be assigned to the case.";
	poster = "MemoriesOfMurder.png";
	film = new Film{ title, releaseYear, director, genres, cast, description, poster };
	m_films.insert(film);
	genres.clear();

	title = "Princess Mononoke";
	director = "Hayao Miyazaki";
	releaseYear = 1997;
	cast[0] = "Yoji Matsuda";
	cast[1] = "Yuriko Ishida";
	cast[2] = "Akihiro Miwa";
	cast[3] = "Yuko Tanaka";
	cast[4] = "Sumi Shimamoto";
	genres.insert(Genre::animation);
	genres.insert(Genre::adventure);
	genres.insert(Genre::drama);
	genres.insert(Genre::war);
	description = "Ashitaka, a prince of the disappearing Emishi people, is cursed by a demonized boar god and must journey to the west to find a cure. Along the way, he encounters San, a young human woman fighting to protect the forest, and Lady Eboshi, who is trying to destroy it. Ashitaka must find a way to bring balance to this conflict.";
	poster = "PrincessMononoke.png";
	film = new Film{ title, releaseYear, director, genres, cast, description, poster };
	m_films.insert(film);
	genres.clear();

	title = "Spirited Away";
	director = "Hayao Miyazaki";
	releaseYear = 2001;
	cast[0] = "Rumi Hiiragi";
	cast[1] = "Miyu Irino";
	cast[2] = "Mari Natsuki";
	cast[3] = "Yumi Tamai";
	cast[4] = "Bunta Sagawara";
	genres.insert(Genre::animation);
	genres.insert(Genre::adventure);
	description = "A young girl, Chihiro, becomes trapped in a strange new world of spirits. When her parents undergo a mysterious transformation, she must call upon the courage she never knew she had to free her family.";
	poster = "SpiritedAway.png";
	film = new Film{ title, releaseYear, director, genres, cast, description, poster };
	m_films.insert(film);
	genres.clear();

	title = "Night on Earth";
	director = "Jim Jarmusch";
	releaseYear = 1991;
	cast[0] = "Winona Ryder";
	cast[1] = "Gena Rowlands";
	cast[2] = "Matti Pellonpaa";
	cast[3] = "Giancarlo Esposito";
	cast[4] = "Isaach De Bankole";
	genres.insert(Genre::comedy);
	genres.insert(Genre::drama);
	description = "An anthology of 5 different cab drivers in 5 American and European cities and their remarkable fares on the same eventful night.";
	poster = "NightOnEarth.png";
	film = new Film{ title, releaseYear, director, genres, cast, description, poster };
	m_films.insert(film);
	genres.clear();

	title = "Se7en";
	director = "David Fincher";
	releaseYear = 1995;
	cast[0] = "Brad Pitt";
	cast[1] = "Morgan Freeman";
	cast[2] = "Kevin Spacey";
	cast[3] = "Gwyneth Paltrow";
	cast[4] = "Emily Wagner";
	genres.insert(Genre::crime);
	genres.insert(Genre::mystery);
	genres.insert(Genre::thriller);
	description = "Two homicide detectives are on a desperate hunt for a serial killer whose crimes are based on the \"seven deadly sins\".The seasoned Det.Sommerset researches each sin in an effort to get inside the killer's mind, while his novice partner, Mills, scoffs at his efforts to unravel the case.";
	poster = "Se7en.png";
	film = new Film{ title, releaseYear, director, genres, cast, description, poster };
	m_films.insert(film);
	genres.clear();

	title = "Style Wars";
	director = "Tony Silver";
	releaseYear = 1983;
	cast[0] = "Rammellzee";
	cast[1] = "Ed Koch";
	cast[2] = "DJ Kay Slay";
	cast[3] = "Butch";
	cast[4] = "Kase";
	genres.insert(Genre::documentary);
	description = "A documentary that exposes the rich growing subculture of hip-hop that was developing in New York City in the late '70s and early '80s, specifically focusing on graffiti art and breakdancing.";
	poster = "StyleWars.png";
	film = new Film{ title, releaseYear, director, genres, cast, description, poster };
	m_films.insert(film);

	m_currentFilms = m_films;
}