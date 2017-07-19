#pragma once

#include <string>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "Texture.h"
#include "Shaders.h"
#include "volume.hpp"

class Raycast
{
public:
	Raycast(int screenWidth, int screenHeight);
	~Raycast();

	void draw();
	void cast();

	void loadVolume(std::string filename);
private:
	glm::vec3 eye;
	float distance;
	glm::vec3 normal;
	glm::vec3 min, max, d;
	glm::vec3 _min, _max, _d;

	Volume *volume = nullptr;
	GLuint buffer;
	Shader computeShader = Shader("Shaders\\raycast.glsl");
	Shader textureShader = Shader("Shaders\\vertex.glsl", "Shaders\\fragment.glsl");
	Texture *texture;
	int screenWidth, screenHeight;
	int closest_side() noexcept;
};