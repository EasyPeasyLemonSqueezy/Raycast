#pragma once

#include <string>
#include <fstream>
#include <iostream>

#include <glew\glew.h>
#include <freeglut\freeglut.h>

class Shader
{
public:
	GLuint program;

	Shader(const std::string& vertexShaderName, const std::string& fragmentShaderName)
	{
		std::string vertexSource = readShader(vertexShaderName);
		std::string fragmentSource = readShader(fragmentShaderName);

		GLuint vertexShader = createShader(GL_VERTEX_SHADER, "vertex shader", vertexSource);
		GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, "fragment shader", fragmentSource);

		program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);

		int linkResult = 0;

		// Ошибки
		glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
		if (linkResult == GL_FALSE)
		{
			int infoLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);
			std::vector<char> programLog(infoLength);
			glGetProgramInfoLog(program, infoLength, NULL, &programLog[0]);
			std::cout << "Link error" << std::endl << &programLog[0] << std::endl;
		}
	}

	Shader(const std::string& computeShaderName)
	{
		std::string computeSource = readShader(computeShaderName);

		GLuint computeShader = createShader(GL_COMPUTE_SHADER, "compute shader", computeSource);

		program = glCreateProgram();
		glAttachShader(program, computeShader);
		glLinkProgram(program);

		int linkResult = 0;

		// Ошибки
		glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
		if (linkResult == GL_FALSE)
		{
			int infoLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);
			std::vector<char> programLog(infoLength);
			glGetProgramInfoLog(program, infoLength, NULL, &programLog[0]);
			std::cout << "Link error" << std::endl << &programLog[0] << std::endl;
		}
	}

	~Shader()
	{
		glDeleteProgram(program);
	}
private:
	std::string readShader(const std::string& filename)
	{
		std::string shaderSource;
		std::ifstream file(filename, std::ios::in);

		if (!file.good())
		{
			std::cout << "Can't read file " << filename << std::endl;
			std::terminate();
		}

		file.seekg(0, std::ios::end);
		shaderSource.resize((unsigned int)file.tellg());
		file.seekg(0, std::ios::beg);
		file.read(&shaderSource[0], shaderSource.size());
		file.close();

		return shaderSource;
	}

	GLuint createShader(GLenum shaderType, const std::string& shaderName, const std::string& source)
	{
		const char* sourcePointer = source.c_str();
		const int sourceSize = source.size();

		GLuint shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &sourcePointer, &sourceSize);
		glCompileShader(shader);

		int compileResult = 0;

		// Ошибки
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
		if (compileResult == GL_FALSE)
		{
			int infoLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);
			std::vector<char> shaderLog(infoLength);
			glGetShaderInfoLog(shader, infoLength, NULL, &shaderLog[0]);
			std::cout << "Error compiling shader " << shaderName << std::endl << &shaderLog[0] << std::endl;
			return 0;
		}

		return shader;
	}
};