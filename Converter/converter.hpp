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

	std::cout << "Reading data..." << std::endl;

	std::tie(volume, x, y, z, min, max) = input<color_t>(fname);

	std::cout << "Size: " << x << 'x' << y << 'x' << z << std::endl;
	std::cout << "Min: (" << min.x << ", " << min.y << ", " << min.z << ')' << std::endl;
	std::cout << "Max: (" << max.x << ", " << max.y << ", " << max.z << ')' << std::endl;

	vec d = {
		(max.x - min.x) / (x - 1),
		(max.y - min.y) / (y - 1),
		(max.z - min.z) / (z - 1)
	};

	std::cout << "Delta: (" << d.x << ", " << d.y << ", " << d.z << ')' << std::endl << std::endl;


	std::cout << "Adjustment..." << std::endl;
	auto result = adjustment<color_t>(x, y, z, d, min, volume);

	std::cout << "Saving result..." << std::endl;
	output<color_t>(fname, result, { x, y, z, min, max, d, color_t::format() });
	
	std::cout << "Done." << std::endl << std::endl;

	delete[] result;
}