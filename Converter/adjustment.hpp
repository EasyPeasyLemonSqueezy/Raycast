#pragma once
#include "color.hpp"
#include "voxel.hpp"
#include <cmath>
#include <cstdint>
#include <vector>


template <typename color_t>
color_t * adjustment(const std::uint64_t x, const std::uint64_t y, const std::uint64_t z,
					 const vec d, const vec min,
					 const std::vector<voxel<color_t>> &volume)
{
	color_t * result = new color_t[x * y * z](); // zero initialized

	for (auto &voxel : volume) {
		const auto xi = static_cast<std::uint64_t>(std::round((voxel.v.x - min.x) / d.x));
		const auto yi = static_cast<std::uint64_t>(std::round((voxel.v.y - min.y) / d.y));
		const auto zi = static_cast<std::uint64_t>(std::round((voxel.v.z - min.z) / d.z));

		const uint64_t index = x * y * zi + x * yi + xi;
		result[index] = voxel.color;
	}

	return result;
}