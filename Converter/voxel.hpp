#pragma once
#include "color.hpp"
#include "vec.hpp"

#pragma pack(push, 1)
template <typename color_t>
struct voxel
{
	vec v;
	color_t color;
};
#pragma pack(pop)