#pragma once
#include "adjustment.hpp"
#include "io.hpp"
#include "vec.hpp"
#include "voxel.hpp"
#include <string>
#include <tuple>
#include <vector>

template <typename color_t>
void convert(const std::string &fname)
{
	std::vector<voxel<color_t>> volume;
	vec min, max;
	uint64_t x, y, z;

	cout << "reading data" << endl;
	std::tie(volume, x, y, z, min, max) = input<color_t>(fname);

	vec d = {
		(max.x - min.x) / (x - 1),
		(max.y - min.y) / (y - 1),
		(max.z - min.z) / (z - 1)
	};

	cout << "adjustment" << endl;
	auto result = adjustment<color_t>(x, y, z, d, min, volume);

	cout << "saving result" << endl;
	output<color_t>(fname, result, { x, y, z, min, max, d, color_t::format() });

	delete[] result;
}