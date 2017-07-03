#pragma once
#include "vec.hpp"
#include "voxel.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include <unordered_set>


template <typename color_t>
std::tuple<std::vector<voxel<color_t>>, std::uint64_t, std::uint64_t, std::uint64_t, vec, vec>
input(const std::string & fname)
{
	namespace fs = std::experimental::filesystem;
	
	std::vector<voxel<color_t>> volume;
	std::unordered_set<float> x, y, z;
	const float inf = std::numeric_limits<float>::infinity();

	vec min{  inf,  inf,  inf },
	    max{ -inf, -inf, -inf };

	std::ifstream in(fs::canonical(fname), std::ios::in);
	if (!in.good()) {
		std::cout << "Error: File not found." << std::endl;
		exit(-1);
	}

	float xc, yc, zc;
	color_t color;

	while (in >> xc >> yc >> zc >> color) {
		volume.push_back({
			{ xc, yc, zc },
			color
		});

		if (x.find(xc) == x.end()) { x.insert(xc); }
		if (y.find(yc) == y.end()) { y.insert(yc); }
		if (z.find(zc) == z.end()) { z.insert(zc); }

		if (min.x > xc) { min.x = xc; }
		if (min.y > yc) { min.y = yc; }
		if (min.z > zc) { min.z = zc; }

		if (max.x < xc) { max.x = xc; }
		if (max.y < yc) { max.y = yc; }
		if (max.z < zc) { max.z = zc; }
	}

	return { volume, x.size(), y.size(), z.size(), min, max };
}