#pragma once
#include "color.hpp"
#include "voxel.hpp"
#include <cstdint>


color * adjustment(const std::uint64_t, const std::uint64_t, const std::uint64_t, // x, y, z
				   const vec, const vec, // min, max
				   const voxel *, const std::uint64_t); // data, count