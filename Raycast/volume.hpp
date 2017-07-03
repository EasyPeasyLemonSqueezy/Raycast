#pragma once
#include "color.hpp"
#include "header.hpp"
#include <optional>
#include <string>
#include <tuple>

#include <glm\glm.hpp>

struct Volume final
{
	Volume(std::string) noexcept;
	~Volume() noexcept;

	header info;
	glm::vec4 *data;
};