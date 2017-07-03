#include "Texture.h"
#include "DebugLog.h"
#include "volume.hpp"

#include <chrono>
#include <iostream>

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 1024;

Texture* texture;

void raycast()
{
	Shader computeShader("Shaders\\raycast.glsl");
	glUseProgram(computeShader.program);
	
	glBindImageTexture(0, texture->textureId, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	Volume volume("C:\\Users\\Anton\\Downloads\\notes_2.txt.vd");
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

	auto start = chrono::steady_clock::now();

	glDispatchCompute(SCREEN_WIDTH, SCREEN_HEIGHT, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	auto end = chrono::steady_clock::now();

	const auto total = chrono::duration <double, milli>(end - start).count();
	cout << "time: " << total << endl;

	glDeleteBuffers(1, &buffer);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(.7f, .0f, 1.f, 1.f);

	texture->draw();

	glFlush();
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

	Shader shader("Shaders\\vertex.glsl", "Shaders\\fragment.glsl");
	texture = new Texture(SCREEN_WIDTH, SCREEN_HEIGHT, shader.program);

	raycast();
	
	glutMainLoop();

	delete texture;

	return 0;
}