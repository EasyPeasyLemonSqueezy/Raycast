#pragma once
#include <cstdint>

#pragma pack(push, 1)
struct color
{
	float opacity;
	std::uint8_t r, g, b;
};
#pragma pack(pop)