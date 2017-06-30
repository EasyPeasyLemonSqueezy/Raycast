#pragma once
#include "vec.hpp"
#include "voxel.hpp"

#include <string>
#include <tuple>

std::tuple<voxel *, uint64_t, vec, vec> input(uint64_t, uint64_t, uint64_t, const std::string &);