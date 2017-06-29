#include "alphaBlend.h"

// top - last point
float blend(std::stack<color> points)
{
	float hue = 0; // Background

	while (!points.empty()) {
		hue *= 1 - points.top().opacity;
		hue += points.top().hue;

		points.pop();
	}

	return hue;
}