#include "volume.h"
#include <cmath>
#include <fstream>

using namespace std;

Volume::Volume(int x, int y, int z, string fname) noexcept
	: x(x), y(y), z(z), xmin(std::numeric_limits<float>::infinity()), ymin(xmin), zmin(xmin)
{
	data = new voxel[x * y * z];

	// Input
	ifstream input(fname, ios::in);
	float xc, yc, zc, color, opacity;

	int i = 0;
	while (input >> xc >> yc >> zc >> color >> opacity) {
		xmin = xc < xmin ? xc : xmin;
		ymin = yc < ymin ? yc : ymin;
		zmin = zc < zmin ? zc : zmin;

		data[i++] = voxel{ xc, yc, zc, color, opacity };
	}

	input.close();

	//// Calculation
	//float dx = .0, dy = .0;

	//for (int i = 0; i < z; ++i) {
	//	// it's not actually correct but I don't care
	//	for (int iz = 1; iz < x * y; ++iz) {
	//		dx += data[i * x * y + iz].x;
	//		dy += data[i * x * y + iz].y;
	//	}
	//}

	//dx /= x * y * z;
	//dy /= x * y * z;

	//// Correction (only x and y)
	//for (int i = 0; i < x * y * z; ++i) {
	//	data[i].x -= dx;
	//	data[i].y -= dy;
	//}

	dx = data[1].x - data[0].x;
	dy = data[x].y - data[0].y;
	dz = data[x * y].z - data[0].z;
}

Volume::~Volume() noexcept
{
	delete[] data;
}

const color & Volume::get(float x, float y, float z)
{
	int xi = round((x - xmin) / dx), yi = round((y - ymin) / dy), zi = round((z - zmin) / dz);

	return data[zi * this->x * this->y + yi * this->x + xi].color;
}

const color & Volume::operator[](const tuple<float, float, float>& coord)
{
	return get(std::get<0>(coord), std::get<1>(coord), std::get<2>(coord));
}
