#pragma once
#include "color.hpp"
#include <cstdint>
#include <tuple>
#include <vector>

std::tuple<std::uint8_t, std::uint8_t, std::uint8_t>
blend(const std::vector<color> &);
