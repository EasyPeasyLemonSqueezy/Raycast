#include "volume.hpp"
#include "header.hpp"
#include "input.hpp"
#include <cmath>
#include <fstream>

using namespace std;


Volume::Volume(string fname) noexcept
{
	ifstream in(fname, ios::in | ios::binary);

	in.read(reinterpret_cast<char *>(&info), sizeof(header));

	if (info.format == color_format::rgba) {
		data = input<color_rgba>(in, info.volume());
	}
	else if (info.format == color_format::ha) {
		data = input<color_ha>(in, info.volume());
	}
}

Volume::~Volume() noexcept
{
	delete[] data;
}