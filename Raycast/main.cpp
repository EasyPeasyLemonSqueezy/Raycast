#include <color.hpp>
#include <converter.hpp>
#include <color.cpp>

#include "DebugLog.h"

#include <windows.h>
#include <direct.h>
#include <Commdlg.h>
#include <Shellapi.h>

#include "Raycast.h"

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 1024;

Raycast* raycast;
glm::vec2 mousePosition;

void chooseAndDrawImage(int id)
{
	OPENFILENAME ofn;
	char fileName[MAX_PATH];

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = fileName;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(fileName);
	ofn.lpstrFilter = "All\0*.*\0.txt\0*.TXT\0.vd\0*.VD\0\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	char CWD[MAX_PATH];
	_getcwd(CWD, MAX_PATH);
	GetOpenFileName(&ofn);
	_chdir(CWD);

	string s(fileName);

	if (s.empty()) {
		return;
	}

	if (id != 0) {
		switch (id) {
		case 1:
			convert<color_ha>(s); break;
		case 2:
			convert<color_rgba>(s); break;
		}

		s += ".vd";
	}

	raycast->loadVolume(s);
}

void menu(int id)
{
	chooseAndDrawImage(id);
	glutPostRedisplay();
}

void createMenu()
{
	int menuId = glutCreateMenu(menu);
	glutAddMenuEntry("Open converted file", 0);
	glutAddMenuEntry("Convert and open ha", 1);
	glutAddMenuEntry("Convert and open rgba", 2);
	glutAttachMenu(GLUT_LEFT_BUTTON);
}

void reshape(int width, int height)
{
	glViewport((width - SCREEN_WIDTH) / 2.0f, (height - SCREEN_HEIGHT) / 2.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void update(int time)
{
	raycast->raycast();
	glutPostRedisplay();
	glutTimerFunc(16, update, 0);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	raycast->draw();

	glFlush();
}

void mouseInput(int x, int y)
{
	const float mouseSensitivity = 0.25f;

	glm::vec2 mouseDelta = glm::vec2(x, y) - mousePosition;
	mouseDelta *= mouseSensitivity;

	raycast->updateAngle(mouseDelta);

	mousePosition = glm::vec2(x, y);
	glutPostRedisplay();
}

void init(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("Volume Ray Casting");

	glutTimerFunc(16, update, 0);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutPassiveMotionFunc(mouseInput);

	if (glewInit())
	{
		cerr << "Unable to initialize GLEW" << endl;
		exit(EXIT_FAILURE);
	}

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(debugCallback, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	std::cout << "OpenGL Debug enabled" << std::endl;
#endif
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	createMenu();

	raycast = new Raycast(SCREEN_WIDTH, SCREEN_HEIGHT);

	glutMainLoop();

	return 0;
}