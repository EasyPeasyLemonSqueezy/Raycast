#include "alphaBlend.hpp"
#include "DebugLog.h"
#include "Texture.h"
#include "volume.hpp"

#include <chrono>
#include <iostream>
#include <future>
#include <vector>

#include <glm\glm.hpp>

using namespace std;

const int SCREEN_WIDTH = 512;
const int SCREEN_HEIGHT = 512;

Volume volume = Volume("");
GLubyte* pixels;
Texture* texture;

void raycast(const Volume& volume)
{
	pixels = new GLubyte[SCREEN_WIDTH * SCREEN_HEIGHT * 3];

	const float eye_position = -100.0f;
	const float monitor_position = -50.0f;
	const float to_volume = volume.info.min.z - eye_position;
	const float to_monitor = monitor_position - eye_position;

	vector<std::future<void>> futures;
	futures.reserve(SCREEN_HEIGHT);

	auto start = chrono::steady_clock::now();
	for (int y = -SCREEN_HEIGHT / 2; y < SCREEN_HEIGHT / 2; ++y)
	{
		futures.push_back(std::async(std::launch::async, [&, y] {
			vector<color> points;
			points.reserve(volume.info.z);

			for (int x = -SCREEN_WIDTH / 2; x < SCREEN_WIDTH / 2; ++x) {
				glm::vec3 ray(x, y, to_monitor);
				ray *= to_volume / to_monitor;

				const glm::vec3 step = ray * (volume.info.d.z / to_volume);
				ray.z = volume.info.min.z; // eye -> ray

				for (uint64_t sample = 0; sample < volume.info.z; ray += step, ++sample) {
					if (auto c = volume.get(ray.x, ray.y, ray.z)) {
						points.push_back(c.value());

						if (c.value().opacity == 1) {
							break;
						}
					}
					else {
						break;
					}
				}

				const auto rc = blend(points);
				const ptrdiff_t index = 3 * (SCREEN_WIDTH * (y + SCREEN_HEIGHT / 2) + x + SCREEN_WIDTH / 2);

				pixels[index] = rc;
				pixels[index + 1] = rc;
				pixels[index + 2] = rc;
			
				points.clear();
			}
		}));
	}

	for (const auto &future : futures) {
		future.wait();
	}

	auto end = chrono::steady_clock::now();
	const auto total = chrono::duration <double, milli>(end - start).count();
	cout << "time: " << total << endl;
}

void display()
{
	static int delta = 0;
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(.7f, .0f, 1.f, 1.f);

	texture->draw();

	glFlush();

	std::cout << "GO\n";

}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Volume Ray Casting");

	glutDisplayFunc(display);

	glewInit();

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(debugCallback, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
#endif
	std::cout << "Start loaded\n";

	raycast(volume);
	std::cout << "Raycast\n";
	texture = new Texture(SCREEN_WIDTH, SCREEN_HEIGHT, pixels);
	std::cout << "Textur\n";

	glutMainLoop();

	delete texture;

	return 0;
}