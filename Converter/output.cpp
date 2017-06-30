#include "output.hpp"
#include <fstream>

using namespace std;

void output(const std::string &fname,
			color const * const volume,
			const header &info)
{
	ofstream out(fname + ".vd", ios::out | ios::binary);

	out.write(reinterpret_cast<const char *>(&info), sizeof(header));
	out.write(reinterpret_cast<const char *>(volume), sizeof(color) * info.volume());
}
