#include "filmutil.h"
#include <string>
#include <sstream>
#include <utility>
#include <vector>
#include <array>

// serves as a "toString" for genres
const std::string genreName(const Genre genre) {
	using enum Genre;		// C++20 - to avoid redundant, repetitive Genre prefixes
	switch (genre) {
	case adventure:
		return "Adventure";
	case animation:
		return "Animation";
	case comedy:
		return "Comedy";
	case crime:
		return "Crime";
	case documentary:
		return "Documentary";
	case drama:
		return "Drama";
	case mystery:
		return "Mystery";
	case romance:
		return "Romance";
	case thriller:
		return "Thriller";
	case war:
		return "War";
	default:
		return "";
	}
}

const std::array<Genre, 10> genresToArray()
{
	using enum Genre;
	return { adventure, animation, comedy, crime, documentary, drama, mystery, romance, thriller, war };
}

/*
const std::vector<std::string> descriptionTokenized(const std::string& description)
{
	
}*/
