#pragma once
#include "widget.h"
#include "film.h"

#include <unordered_set>

/*
 * A slider will be one of 2 types - a lower bound slider (disregards all the values lower than it's currently at), and an upper bound slider
 * (diregards all the values higher than it's currently at). Therefore, making a custom enum regarding a slider's useage, with those 2 values
 */
enum class SliderUseage { LOWER_BOUND, UPPER_BOUND };

class Slider : public Widget {
private:

	// A slider is rectangular, therefore it will have its own width/height variables.
	float m_width;
	float m_height;

	// m_lowerBound, m_upperBound: Signify the coordinates at the smallest and greatest points of the line drawn behind the slider.
	float m_lowerBound;
	float m_upperBound;

	// m_lineWidth: Signifies the width of the line to be drawn behind the slider
	float m_lineWidth;

	// m_OriginalCentre: used to mark the pos_x passed as the centre of the slider to the slider on instantiation.
	// This represents the horizontal position that marks the centre of the line that will be drawn behind our slider.
	float m_originalCentre;

	// each of the 2 sliders will have its own useage, so they can filter results accurately
	const SliderUseage m_useage;
	
	// m_minValue, m_maxValue: Signify the absolute values at each end of the slider. Naming them min/max "Values" instead of
	// "Years" to make the code slightly more "expandable"
	unsigned int m_minValue;
	unsigned int m_maxValue;

public:
	Slider(const float pos_x, const float pos_y, const float width, const float height, const unsigned int minValue,
		const unsigned int maxValue, const SliderUseage useage);

	void draw() override;
	void update() override;

	// fetches the value (year in our case) the current location of the slider represents
	unsigned int getCurrentValue() const;

	// both sliders active in the search screen will be filtering the results no matter what, therefore making canFilter return true always
	bool canFilter() const override;

	// The lower bound slider will discard the films with a release date prior to its current value, and likewise for the upper bound slider.
	virtual void filter(std::unordered_set<class Film*>& currFilms) const override;

	// a slider is rectangular, therefore its contains will utilise our "rectangularContains" helper function.
	bool contains(const float x, const float y) const override;
};