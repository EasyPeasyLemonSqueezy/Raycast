#include "Texture.h"
#include "DebugLog.h"
#include "volume.h"
#include <iostream>

#include <glm\glm.hpp>

const int SCREEN_WIDTH = 256;
const int SCREEN_HEIGHT = 256;

GLubyte* pixels;
Texture* texture;

void createPixels()
{
	pixels = new GLubyte[SCREEN_HEIGHT * SCREEN_WIDTH * 3];

	for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH * 3; i += 3)
	{
		int color = rand() % 255;
		pixels[i] = color;
		pixels[i + 1] = color;
		pixels[i + 2] = color;
	}
}

void raycast(const Volume& volume)
{
	pixels = new GLubyte[SCREEN_WIDTH * SCREEN_HEIGHT * 3];
	int k = 0;

	glm::vec3 eye(0.0f, 0.0f, -100.0f);
	float dz = 0.625f;
	const float zmin = 5.625;
#pragma omp parallel
#pragma omp for
	for (int i = -SCREEN_HEIGHT / 2; i < SCREEN_HEIGHT / 2; ++i)
	{
		std::cout << i << std::endl;
#pragma omp for
		for (int j = -SCREEN_WIDTH / 2; j < SCREEN_WIDTH / 2; ++j)
		{
			/*glm::vec3 pixel(i, j, -50.0f);
			glm::vec3 ray = pixel - eye;
			glm::normalize(ray);*/

			float y = i * ((zmin + 100) / 50);
			float x = j * ((zmin + 100) / 50);
			float z = zmin;

			float dx = j * ((zmin + 100 + dz) / 50) - x;
			float dy = i * ((zmin + 100 + dz) / 50) - y;

			std::stack<color> points;
			for (float z = zmin; z <= 15.625; z += dz)
			{
				color c;
				try {
					c = volume.get(x, y, z);
					points.push(c);
				}
				catch (const std::exception &e) {
					break;
				}
			}

			auto rc = blend(points);
			
			pixels[k++] = rc;
			pixels[k++] = rc;
			pixels[k++] = rc;
		}
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.7, 0.0, 1.0, 1.0);

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
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("Volume Ray Casting");

	glutDisplayFunc(display);

	glewInit();

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(debugCallback, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
#endif
	std::cout << "Start loaded\n";

	//createPixels();
	Volume volume(512, 512, 17, "C:\\Users\\Obir\\Desktop\\notes_c.txt");
	std::cout << "Volume loaded\n";
	raycast(volume);
	std::cout << "Raycast\n";
	texture = new Texture(SCREEN_WIDTH, SCREEN_HEIGHT, pixels);
	std::cout << "Textur\n";

	glutMainLoop();

	delete texture;

	return 0;
}