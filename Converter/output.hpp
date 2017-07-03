#pragma once
#include "color.hpp"
#include "header.hpp"

#include <filesystem>
#include <fstream>
#include <string>


template <typename color_t>
void output(const std::string & fname,
			color_t const * const volume,
			const header &info)
{
	namespace fs = experimental::filesystem;

	std::ofstream out(fs::canonical(fname + ".vd"), std::ios::out | std::ios::binary);

	out.write(reinterpret_cast<const char *>(&info), sizeof(header));
	out.write(reinterpret_cast<const char *>(volume), sizeof(color_t) * info.volume());
}