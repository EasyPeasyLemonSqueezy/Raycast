#pragma once
#include "color.hpp"
#include "header.hpp"
#include <optional>
#include <string>
#include <tuple>

class Volume
{
public:
	Volume(std::string) noexcept;
	~Volume() noexcept;
	std::optional<color> get(float, float, float) const noexcept;
	header info;

	float* opacities;
	float* hues;
private:
	color *data;
};