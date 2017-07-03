#pragma once
#include <cstdint>
#include <sstream>


enum class color_format : std::uint8_t { rgba, ha };

#pragma pack(push, 1)
struct color_rgba
{
	float opacity;
	std::uint8_t r, g, b;

	static auto format() { return color_format::rgba; }
};
#pragma pack(pop)

std::istream & operator >> (std::istream &is, color_rgba &c);


#pragma pack(push, 1)
struct color_ha
{
	float opacity;
	std::uint8_t hue;

	static auto format() { return color_format::ha; }
};
#pragma pack(pop)

std::istream & operator >> (std::istream &is, color_ha &c);