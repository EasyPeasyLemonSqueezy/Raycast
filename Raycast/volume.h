#pragma once
#include "alphaBlend.h"
#include "voxel.h"
#include <string>
#include <tuple>


class Volume
{
public:
	Volume(int, int, int, std::string) noexcept;
	~Volume() noexcept;
	const color & get(float, float, float);
	const color & operator[] (const std::tuple<float, float, float> &);

	const int x, y, z;
private:
	voxel *data;
	float dx, dy, dz;
	float xmin, ymin, zmin;
};