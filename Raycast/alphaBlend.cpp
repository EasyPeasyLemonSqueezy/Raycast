#include "alphaBlend.hpp"

using namespace std;

// top - last point
uint8_t blend(const vector<color> &points)
{
	float hue = 0; // Background

	for (auto &it = points.rbegin(); it != points.rend(); ++it) {
		hue = it->hue * it->opacity + hue * (1 - it->opacity);
	}

	return static_cast<uint8_t>(hue);
}