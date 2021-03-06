#include <color.hpp>
#include <converter.hpp>
#include <color.cpp>

#include "Texture.h"
#include "DebugLog.h"
#include "volume.hpp"

#include <chrono>
#include <iostream>

#include <windows.h>
#include <direct.h>
#include <Commdlg.h>
#include <Shellapi.h>

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 1024;

Texture* texture;

void raycast(const string &fname)
{
	Shader computeShader("Shaders\\raycast.glsl");
	glUseProgram(computeShader.program);
	
	glBindImageTexture(0, texture->textureId, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	Volume volume(fname);
	const uint64_t volumeSize = sizeof(float) * volume.info.volume();
	header info = volume.info;

	GLuint buffer;
	glGenBuffers(1, &buffer);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, volumeSize * 4, volume.data, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, buffer);

	glUniform1f(glGetUniformLocation(computeShader.program, "eyePosition"), -500.0f);
	glUniform1f(glGetUniformLocation(computeShader.program, "monitorPosition"), 0.0f);
	glUniform2i(glGetUniformLocation(computeShader.program, "screen"), SCREEN_WIDTH, SCREEN_HEIGHT);
	glUniform3i(glGetUniformLocation(computeShader.program, "size"), static_cast<GLint>(info.x), static_cast<GLint>(info.y), static_cast<GLint>(info.z));
	glUniform3f(glGetUniformLocation(computeShader.program, "d"), info.d.x, info.d.y, info.d.z);
	glUniform3f(glGetUniformLocation(computeShader.program, "min"), info.min.x, info.min.y, info.min.z);
	glUniform3f(glGetUniformLocation(computeShader.program, "max"), info.max.x, info.max.y, info.max.z);

	const auto start = chrono::high_resolution_clock::now();

	glDispatchCompute(SCREEN_WIDTH, SCREEN_HEIGHT, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	const auto end = chrono::high_resolution_clock::now();

	const auto total = chrono::duration<double, milli>(end - start).count();
	cout << "time: " << total << "ms" << endl;

	glDeleteBuffers(1, &buffer);
}

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

	raycast(s);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	texture->draw();

	glFlush();
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

void init(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("Volume Ray Casting");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

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

	Shader shader("Shaders\\vertex.glsl", "Shaders\\fragment.glsl");
	texture = new Texture(SCREEN_WIDTH, SCREEN_HEIGHT, shader.program);

	glutMainLoop();

	delete texture;

	return 0;
}