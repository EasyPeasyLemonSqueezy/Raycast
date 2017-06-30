#pragma once
#include <cstdint>

#pragma pack(push, 1)
struct color
{
	float opacity;
	std::uint8_t hue;
};
#pragma pack(pop)