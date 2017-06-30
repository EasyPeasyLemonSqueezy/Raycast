#include "adjustment.hpp"
#include <cmath>

using namespace std;

color * adjustment(const uint64_t x, const uint64_t y, const uint64_t z,
				   const vec d, const vec min,
				   const voxel * volume, const uint64_t count)
{
	color * result = new color[x * y * z](); // zero initialized

	for (const voxel *end = volume + count; volume != end; ++volume) {
		const auto xi = static_cast<uint64_t>(round((volume->v.x - min.x) / d.x));
		const auto yi = static_cast<uint64_t>(round((volume->v.y - min.y) / d.y));
		const auto zi = static_cast<uint64_t>(round((volume->v.z - min.z) / d.z));

		const uint64_t index = x * y * zi + x * yi + xi;
		result[index] = volume->color;
	}

	return result;
}
