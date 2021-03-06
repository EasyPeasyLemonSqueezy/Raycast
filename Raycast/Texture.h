#pragma once

#include <glew\glew.h>
#include <freeglut\freeglut.h>

#include "Shaders.h"

struct Texture
{
	GLuint vao;
	GLuint vbo;
	GLuint textureId;
	GLuint shaderProgram;

	Texture(int width, int height, GLuint shaderProgram)
	{
		this->shaderProgram = shaderProgram;
		
		const float buffer[16] =
		{
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f,  1.0f,  1.0f, 1.0f,
		};

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, &buffer[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));

		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}

	~Texture()
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteTextures(1, &textureId);
	}

	void draw()
	{
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glUniform1i(glGetUniformLocation(shaderProgram, "image"), 0);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
};