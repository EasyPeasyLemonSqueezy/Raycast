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
	void raycast();

	void loadVolume(std::string filename);
	void updateAngle(glm::vec2 mouseDelta);
private:
	glm::vec3 getCameraPosition(float distance, float angleX, float angleY);

	Volume* volume = nullptr;
	GLuint buffer;
	Shader computeShader = Shader("Shaders\\raycast.glsl");
	Shader textureShader = Shader("Shaders\\vertex.glsl", "Shaders\\fragment.glsl");
	Texture* texture;
	int screenWidth, screenHeight;
	glm::vec2 cameraRotation;
};