#pragma once
#include <cstdint>
#include <sstream>


enum class color_format : std::uint8_t { rgba, ha };

struct f_rgba { float r, g, b, a; };

#pragma pack(push, 1)
struct color_rgba
{
	float opacity;
	std::uint8_t r, g, b;

	static auto format() { return color_format::rgba; }
	f_rgba rgba() { return { r / 255.f, g / 255.f, b / 255.f, opacity }; }
};
#pragma pack(pop)

std::istream & operator >> (std::istream &is, color_rgba &c);


#pragma pack(push, 1)
struct color_ha
{
	float opacity;
	std::uint8_t hue;

	static auto format() { return color_format::ha; }
	f_rgba rgba() { float h = hue / 255.f; return { h, h, h, opacity }; }
};
#pragma pack(pop)

std::istream & operator >> (std::istream &is, color_ha &c);