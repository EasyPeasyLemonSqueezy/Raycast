#include "color.hpp"
#include "converter.hpp"

#include <iostream>
#include <string>

using namespace std;


int main(int argc, char *argv[])
{
	if (argc != 3) {
		return -1;
	}

	if (argv[1] == "rgba"s) {
		convert<color_rgba>(argv[2]);
	}
	else if (argv[1] == "ha"s) {
		convert<color_ha>(argv[2]);
	}
	else {
		return -1;
	}

	return 0;
}