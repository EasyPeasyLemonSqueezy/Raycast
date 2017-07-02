#include "alphaBlend.hpp"

using namespace std;

tuple<uint8_t, uint8_t, uint8_t> blend(const vector<color> &points)
{
	uint8_t r = 0, g = 0, b = 0; // Background

	for (auto &it = points.rbegin(); it != points.rend(); ++it) {
		r = static_cast<uint8_t>(it->r * it->opacity + r * (1 - it->opacity));
		g = static_cast<uint8_t>(it->g * it->opacity + g * (1 - it->opacity));
		b = static_cast<uint8_t>(it->b * it->opacity + b * (1 - it->opacity));
	}

	return { r, g, b };
}