#include "raycast.hpp"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#define _USE_MATH_DEFINES

#include <math.h>
#include <chrono>
#include <algorithm>
#include <iostream>
#include <array>
#include <tuple>
#include <functional>

using namespace std;
using namespace glm;

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

void Raycast::cast()
{
	if (volume == nullptr) {
		return;
	}

	const auto start = std::chrono::high_resolution_clock::now();

	glUseProgram(computeShader.program);

	glBindImageTexture(0, texture->textureId, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	header info = volume->info;

	glm::vec3 eye(0);
	float r = 201;
	float monitor = 200.f;

	closest_side();
	cout << "min: " << _min.x << ' ' << _min.y << ' ' << _min.z << endl
	     << "max: " << _max.x << ' ' << _max.y << ' ' << _max.z << endl
	     << "d: "   << _d.x << ' '   << _d.y << ' '   << _d.z << endl;

	glUniform3fv(glGetUniformLocation(computeShader.program, "eye"), 1, value_ptr(eye));
	glUniform1f(glGetUniformLocation(computeShader.program, "distance"), r);
	glUniform3fv(glGetUniformLocation(computeShader.program, "normal"), 1, value_ptr(normal));
	glUniform1f(glGetUniformLocation(computeShader.program, "monitor"), monitor);
	glUniform2i(glGetUniformLocation(computeShader.program, "screen"), screenWidth, screenHeight);
	glUniform3i(glGetUniformLocation(computeShader.program, "size"), static_cast<GLint>(info.x), static_cast<GLint>(info.y), static_cast<GLint>(info.z)); // rewrite.
	glUniform3fv(glGetUniformLocation(computeShader.program, "d"), 1, value_ptr(_d));
	glUniform3fv(glGetUniformLocation(computeShader.program, "min"), 1, value_ptr(_min));
	glUniform3fv(glGetUniformLocation(computeShader.program, "max"), 1, value_ptr(_max));
	glUniformMatrix4fv(glGetUniformLocation(computeShader.program, "rot"), 1, false, value_ptr(toMat4(quat(eye))));


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

	auto conv = [](const vec &v) -> vec3 { return vec3(v.x, v.y, v.z); };
	min = conv(volume->info.min);
	max = conv(volume->info.max);
	d = conv(volume->info.d);

	glDeleteBuffers(1, &buffer);
	glGenBuffers(1, &buffer);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * volume->info.volume() * 4, volume->data, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, buffer);
}

int Raycast::closest_side() noexcept
{
	vec3 start = -eye * distance;

	vector<vec3> sides = {
		vec3((min.x + max.x) / 2, (min.y + max.y) / 2,  min.z),
		vec3((min.x + max.x) / 2, (min.y + max.y) / 2,  max.z),
		vec3( min.x,              (min.y + max.y) / 2, (min.z + max.z) / 2),
		vec3( max.x,              (min.y + max.y) / 2, (min.z + max.z) / 2),
		vec3((min.x + max.x) / 2,        min.y,        (min.z + max.z) / 2),
		vec3((min.x + max.x) / 2,        max.y,        (min.z + max.z) / 2)
	};

	for (auto &s : sides) {
		s -= start;
	}

	auto min_s = std::min(sides.begin(), sides.end(), [] (auto l, auto r) { return l->length() < r->length(); });
	auto side = std::distance(sides.begin(), min_s);


	switch (side) {
	case 0: _min = min;                       _max = max;                       _d = d;                      normal = vec3( 0,  0, -1); break;
	case 1: _min = vec3(max.x, min.y, max.z); _max = vec3(min.x, max.y, min.z); _d = vec3(-d.x,  d.y, -d.z); normal = vec3( 0,  0,  1); break;
	case 2: _min = vec3(min.x, min.y, max.z); _max = vec3(max.x, max.y, min.z); _d = vec3(-d.z,  d.y,  d.x); normal = vec3(-1,  0,  0); break;
	case 3: _min = vec3(max.x, min.y, min.z); _max = vec3(min.x, max.y, max.z); _d = vec3( d.z,  d.y, -d.x); normal = vec3( 1,  0,  0); break;
	case 4: _min = vec3(min.x, min.y, max.z); _max = vec3(max.x, max.y, min.z); _d = vec3( d.x, -d.z,  d.y); normal = vec3( 0, -1,  0); break;
	case 5: _min = vec3(min.x, max.y, min.z); _max = vec3(max.x, min.y, max.z); _d = vec3( d.x,  d.z, -d.y); normal = vec3( 0,  1,  0); break;
	}
	

	return side;
}