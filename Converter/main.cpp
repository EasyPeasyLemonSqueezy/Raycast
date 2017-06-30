#include "adjustment.hpp"
#include "io.hpp"

#include <cstdint>
#include <iostream>
#include <string>

using namespace std;


int main(int argc, char *argv[])
{
	if (argc != 5) {
		return -1;
	}

	uint64_t x = atoi(argv[1]),
	         y = atoi(argv[2]),
	         z = atoi(argv[3]);

	voxel *volume;
	vec min, max;
	uint64_t count;

	cout << "reading data" << endl;
	tie(volume, count, min, max) = input(x, y, z, argv[4]);

	vec d = {
		(max.x - min.x) / (x - 1),
		(max.y - min.y) / (y - 1),
		(max.z - min.z) / (z - 1)
	};

	cout << "adjustment" << endl;
	auto result = adjustment(x, y, z, d, min, volume, count);
	delete[] volume;

	cout << "saving result" << endl;
	output(argv[4], result, { x, y, z, min, max, d });
	delete[] result;


	return 0;
}