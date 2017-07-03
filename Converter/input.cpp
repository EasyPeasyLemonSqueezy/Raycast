#include "input.hpp"

#include <filesystem>
#include <fstream>

using namespace std;
namespace fs = experimental::filesystem;

tuple<voxel *, uint64_t, vec, vec> input(const uint64_t x, const uint64_t y, const uint64_t z, const string &fname)
{
	voxel *volume = new voxel[x * y * z];
	uint64_t count = 0;
	const float inf = numeric_limits<float>::infinity();

	vec min {  inf,  inf,  inf },
		max { -inf, -inf, -inf };

	ifstream in(fs::canonical(fname), ios::in);
	float xc, yc, zc, opacity;
	uint32_t r, g, b;

	while (in >> xc >> yc >> zc >> r >> g >> b >> opacity) {
		volume[count++] = {
			{
				opacity, 
				static_cast<uint8_t>(r),
				static_cast<uint8_t>(g),
				static_cast<uint8_t>(b)
			},
			{ xc, yc, zc }
		};

		if (min.x > xc) { min.x = xc; }
		if (min.y > yc) { min.y = yc; }
		if (min.z > zc) { min.z = zc; }

		if (max.x < xc) { max.x = xc; }
		if (max.y < yc) { max.y = yc; }
		if (max.z < zc) { max.z = zc; }
	}

	return { volume, count, min, max };
}
