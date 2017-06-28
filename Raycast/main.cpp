#include "Texture.h"
#include "DebugLog.h"

const unsigned int SCREEN_WIDTH = 512;
const unsigned int SCREEN_HEIGHT = 512;

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

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.7, 0.0, 1.0, 1.0);

	texture->draw();

	glFlush();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(4, 5);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("Volume Ray Casting");

	glutDisplayFunc(display);

	glewInit();

	createPixels();
	texture = new Texture(SCREEN_WIDTH, SCREEN_HEIGHT, GL_UNSIGNED_BYTE, pixels);

	// Дебаг информация на всякий случай
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(debugCallback, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

	glutMainLoop();

	delete texture;

	return 0;
}