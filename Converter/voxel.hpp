#pragma once
#include "color.hpp"
#include "vec.hpp"

#pragma pack(push, 1)
struct voxel
{
	color color;
	vec v;
};
#pragma pack(pop)