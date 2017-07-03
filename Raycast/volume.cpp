#include "volume.hpp"
#include "header.hpp"
#include <cmath>
#include <fstream>

using namespace std;

#include <iostream>

Volume::Volume(string fname) noexcept
{
	ifstream input(fname, ios::in | ios::binary);

	input.read(reinterpret_cast<char *>(&info), sizeof(header));

	data = new color[info.volume()];

	input.read(reinterpret_cast<char *>(data), sizeof(color) * info.volume());

	colors = new float[info.volume() * 2];
	for (int i = 0; i < info.volume(); i++)
	{
		colors[i * 2] = float(data[i].hue) / 255.0f;
		colors[i * 2 + 1] = data[i].opacity;
	}
}

Volume::~Volume() noexcept
{
	delete[] data;
}

optional<color> Volume::get(float xc, float yc, float zc) const noexcept
{
	auto xi = static_cast<uint64_t>(round((xc - info.min.x) / info.d.x)),
	     yi = static_cast<uint64_t>(round((yc - info.min.y) / info.d.y)),
	     zi = static_cast<uint64_t>(round((zc - info.min.z) / info.d.z));

	if (xi >= info.x || xi < 0 ||
		yi >= info.y || yi < 0 ||
		zi >= info.z || zi < 0) {
		return {};
	}

	const size_t i = (zi * info.x * info.y) + (yi * info.x) + xi;
	return make_optional(data[i]);
}