#pragma once
#include "color.hpp"
#include "vec.hpp"
#include <cstdint>

#pragma pack(push, 1)
struct header
{
	std::uint64_t x, y, z;
	vec min, max;
	vec d;
	color_format format;

	std::uint64_t volume() const noexcept { return x * y * z; }
};
#pragma pack(pop)