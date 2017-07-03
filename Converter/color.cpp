#include "color.hpp"

using namespace std;


istream & operator >> (istream &is, color_rgba &c)
{
	uint32_t r, g, b;
	float opacity;

	is >> r >> g >> b >> opacity;
	c = {
		opacity,
		static_cast<uint8_t>(r),
		static_cast<uint8_t>(g), 
		static_cast<uint8_t>(b)
	};

	return is;
}

istream & operator >> (istream &is, color_ha &c)
{
	uint32_t hue;
	is >> hue >> c.opacity;
	c.hue = hue;

	return is;
}