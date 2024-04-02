#pragma once
#include <string>
#include <sstream>
#include <utility>
#include <vector>
#include <array>

/* filmutil.h: Includes an enum class for the genres in our browser and many helper functions for said enum
 * class (like a function that returns the name of each genre and a function that returns all genres).	*/

enum class Genre {
	adventure,
	animation,
	comedy,
	crime,
	documentary,
	drama,
	mystery,
	romance,
	thriller,
	war,
};

// serves as a "toString" for genres
const std::string genreName(const Genre genre);		// Genre is an enum class, therefore it is cheaper to pass by value than by reference

// provides us with an array including all genres. Utilised in the forEach loop in filmUI when creating genre buttons
const std::array<Genre, 10> genresToArray();