# FilmGUI

## Description

This repository contains the project I developed for AUEB's "Programming in C++" (INF232 - 3321) course, which I took in the winter semester of the 2022-23 academic year. The aim was to create a simple graphical interface for a hypothetical film browser, while making sole use of the [SGG library](https://github.com/cgaueb/sgg) for all graphical purposes.

This project was developed over approximately 2 weeks between late December 2022 and early January 2023. No version control system was used during development, meaning there is no commit history for the project.

## Widgets

The following widgets were implemented from scratch:

- **Sliders**: Two sliders were implemented in the advanced search page, which can be used to set custom lower and upper bounds for the release year of a film.

- **Text Fields**:
  - <u>Main Menu Search Bar</u>: The main menu search bar enables a user to filter films without heading to the advanced search page. It filters based on a match on a film's title, director, or any cast member.
  - <u>Advanced Search Text Fields</u>: There are three text fields available in the advanced search page. They allow filtering based on a film's title, director, actor, or any combination thereof.
- **Buttons**:
  - <u>Genre Buttons</u>: Filtering based on genres is also allowed in the advanced search page. This can be achieved by clicking (and de-clicking) on any the buttons that correspond to the provided genres.
  - <u>Apply/Clear Filters</u>: Two rudimentary buttons were also included in the advanced search page, in order to apply and clear search filters respectively.

## Installation

To install the project and run the executable, follow these steps:

1. Clone the repository to your local machine.
2. Navigate to the `bin` folder.
3. Locate `FilmGUI.exe` and execute it.

## Roadmap

- Utilise an XML library for efficient loading.
- Increase display limit, by allowing the application to display more than the current limit of ten films.

## Credits

Film details provided by:

- [Letterboxd](https://letterboxd.com/)
- [IMDb](https://www.imdb.com/)

## License

[MIT](https://choosealicense.com/licenses/mit/)
