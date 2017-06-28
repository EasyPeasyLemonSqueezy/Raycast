#pragma once

#include <glew\glew.h>
#include <freeglut\freeglut.h>

void CALLBACK debugCallback(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* data
);