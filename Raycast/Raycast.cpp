#include "Raycast.h"

#define _USE_MATH_DEFINES

#include <math.h>
#include <chrono>
#include <iostream>

Raycast::Raycast(int screenWidth, int screenHeight) : screenWidth(screenWidth), screenHeight(screenHeight)
{
	texture = new Texture(screenWidth, screenHeight, textureShader.program);
}

Raycast::~Raycast()
{
	delete volume;
	delete texture;
	glDeleteBuffers(1, &buffer);
}

void Raycast::draw()
{
	texture->draw();
}

void Raycast::raycast()
{
	if (volume == nullptr)
	{
		return;
	}

	glUseProgram(computeShader.program);

	glBindImageTexture(0, texture->textureId, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	header info = volume->info;

	auto camPos = cameraRotation(300.0f, 50.0f, 0.0f);
	auto rotation = glm::lookAt(camPos, glm::vec3(0.0f, 50.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glUniform3f(glGetUniformLocation(computeShader.program, "eyePosition"), camPos.x, camPos.y, camPos.z);
	glUniform1f(glGetUniformLocation(computeShader.program, "eyeDistance"), -300.0f);
	glUniform1f(glGetUniformLocation(computeShader.program, "monitorDistance"), -100.0f);
	glUniform2i(glGetUniformLocation(computeShader.program, "screen"), screenWidth, screenHeight);
	glUniform3i(glGetUniformLocation(computeShader.program, "size"), static_cast<GLint>(info.x), static_cast<GLint>(info.y), static_cast<GLint>(info.z));
	glUniform3f(glGetUniformLocation(computeShader.program, "d"), info.d.x, info.d.y, info.d.z);
	glUniform3f(glGetUniformLocation(computeShader.program, "min"), info.min.x, info.min.y, info.min.z);
	glUniform3f(glGetUniformLocation(computeShader.program, "max"), info.max.x, info.max.y, info.max.z);
	glUniformMatrix4fv(glGetUniformLocation(computeShader.program, "rotation"), 1, false, &rotation[0][0]);

	const auto start = std::chrono::high_resolution_clock::now();

	glDispatchCompute(screenWidth, screenHeight, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	const auto end = std::chrono::high_resolution_clock::now();

	const auto total = std::chrono::duration<double, std::milli>(end - start).count();
	std::cout << "time: " << total << "ms" << std::endl;
}

void Raycast::loadVolume(std::string filename)
{
	if (volume != nullptr)
	{
		delete volume;
	}

	volume = new Volume(filename);

	glDeleteBuffers(1, &buffer);
	glGenBuffers(1, &buffer);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * volume->info.volume() * 4, volume->data, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, buffer);
}

glm::vec3 Raycast::cameraRotation(float distance, float angleX, float angleY)
{
	glm::vec3 cameraPosition;

	cameraPosition.x = distance * -sinf(angleX * (M_PI / 180.0f)) * cosf(angleY * (M_PI / 180.0f));
	cameraPosition.y = distance * -sinf(angleY * (M_PI / 180.0f));
	cameraPosition.z = -distance * cosf(angleX * (M_PI / 180.0f)) * cosf(angleY * (M_PI / 180.0f));

	return cameraPosition;
}