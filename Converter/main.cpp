#include "color.hpp"
#include "converter.hpp"

#include <iostream>
#include <string>

using namespace std;


int main(int argc, char *argv[])
{
	if (argc != 3) {
		cout << "Should be only 2 arguments: [rgba|ha] path" << endl;
		return -1;
	}

	if (argv[1] == "rgba"s) {
		convert<color_rgba>(argv[2]);
	}
	else if (argv[1] == "ha"s) {
		convert<color_ha>(argv[2]);
	}
	else {
		cout << "Unknown color format" << endl;
		return -1;
	}

	return 0;
}