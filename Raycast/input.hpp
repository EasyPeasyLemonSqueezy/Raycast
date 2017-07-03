#pragma once
#include <cstdint>
#include <fstream>
#include <glm\vec4.hpp>


template <typename color_t>
glm::vec4 *input(std::ifstream &input, std::uint64_t size)
{
	auto *raw_data = new color_t[size];
	input.read(reinterpret_cast<char *>(raw_data), sizeof(color_t) * size);

	auto *data = new glm::vec4[size];

	for (int i = 0; i < size; ++i) {
#pragma warning(suppress: 6385)
		const auto c = raw_data[i].rgba();
		data[i] = glm::vec4(c.r, c.g, c.b, c.a);
	}

	delete[] raw_data;
	return data;
}