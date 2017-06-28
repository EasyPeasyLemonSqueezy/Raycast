#pragma once

#include <string>

#include <glew\glew.h>
#include <freeglut\freeglut.h>

GLuint createProgram(const std::string& vertexShaderName, const std::string& fragmentShaderName);