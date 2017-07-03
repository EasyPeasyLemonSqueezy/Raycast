#include "volume.hpp"
#include "header.hpp"
#include <cmath>
#include <fstream>

using namespace std;


Volume::Volume(string fname) noexcept
{
	ifstream input(fname, ios::in | ios::binary);

	input.read(reinterpret_cast<char *>(&info), sizeof(header));

	auto *raw_data = new color_rgba[info.volume()];
	input.read(reinterpret_cast<char *>(raw_data), sizeof(color_rgba) * info.volume());

	data = new glm::vec4[info.volume()];

	for (int i = 0; i < info.volume(); ++i) {
#pragma warning(suppress: 6385)
		data[i] = glm::vec4(
			raw_data[i].r / 255.0f,
			raw_data[i].g / 255.0f,
			raw_data[i].b / 255.0f,
			raw_data[i].opacity
		);
	}

	delete[] raw_data;
}

Volume::~Volume() noexcept
{
	delete[] data;
}