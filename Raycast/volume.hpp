#pragma once
#include "color.hpp"
#include "header.hpp"
#include <optional>
#include <string>
#include <tuple>

#include <glm\glm.hpp>

class Volume
{
public:
	Volume(std::string) noexcept;
	~Volume() noexcept;
	std::optional<color> get(float, float, float) const noexcept;
	header info;

	glm::vec4* colors;
private:
	color *data;
};