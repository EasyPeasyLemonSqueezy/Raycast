#include "alphaBlend.hpp"
#include "Texture.h"
#include "volume.hpp"
#include "DebugLog.h"

#include <chrono>
#include <iostream>
#include <future>
#include <vector>

#include <glm\glm.hpp>

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 1024;

GLubyte* pixels;
Texture* texture;

void raycast(const Volume& volume)
{
	pixels = new GLubyte[SCREEN_WIDTH * SCREEN_HEIGHT];

	const float eye_position = -500.0f;
	const float monitor_position = 0.0f;
	const float to_volume = volume.info.min.z - eye_position;
	const float to_monitor = monitor_position - eye_position;

	vector<std::future<void>> futures;
	futures.reserve(SCREEN_HEIGHT);

	auto start = chrono::steady_clock::now();
	for (int y = -SCREEN_HEIGHT / 2; y < SCREEN_HEIGHT / 2; ++y)
	{
		futures.push_back(std::async(std::launch::async, [&, y] {
			vector<color> points;
			points.reserve(volume.info.z);

			for (int x = -SCREEN_WIDTH / 2; x < SCREEN_WIDTH / 2; ++x) {
				glm::vec3 ray(x, y, to_monitor);
				ray *= to_volume / to_monitor;

				const glm::vec3 step = ray * (volume.info.d.z / to_volume);
				ray.z = volume.info.min.z; // eye -> ray

				for (uint64_t sample = 0; sample < volume.info.z; ray += step, ++sample) {
					if (auto c = volume.get(ray.x, ray.y, ray.z)) {
						points.push_back(c.value());

						if (c.value().opacity == 1) {
							break;
						}
					}
					else {
						break;
					}
				}

				const auto rc = blend(points);
				const ptrdiff_t index = SCREEN_WIDTH * (y + SCREEN_HEIGHT / 2) + x + SCREEN_WIDTH / 2;

				pixels[index] = rc;
			
				points.clear();
			}
		}));
	}

	for (const auto &future : futures) {
		future.wait();
	}

	auto end = chrono::steady_clock::now();
	const auto total = chrono::duration <double, milli>(end - start).count();
	cout << "time: " << total << endl;
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

	glewInit();

	/*Volume volume = Volume("D:\\Downloads\\notes_txt\\notes.txt.vd");
	raycast(volume);
*/

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(debugCallback, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

	std::cout << "OpenGL Debug enabled" << std::endl;
#endif

	Shader computeShader("Shaders\\raycast.glsl");
	glUseProgram(computeShader.program);

	Shader shader("Shaders\\vertex.glsl", "Shaders\\fragment.glsl");
	texture = new Texture(SCREEN_WIDTH, SCREEN_HEIGHT, shader.program, NULL);
	glBindImageTexture(0, texture->textureId, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	
	Volume volume = Volume("D:\\Downloads\\notes_txt\\notes.txt.vd");
	const unsigned int volumeSize = sizeof(float) * volume.info.volume();

	GLuint buffers[2];
	glGenBuffers(2, &buffers[0]);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffers[0]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, volumeSize, volume.opacities, GL_STREAM_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, buffers[0]);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffers[1]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, volumeSize, volume.hues, GL_STREAM_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, buffers[1]);

	glUniform1i(glGetUniformLocation(computeShader.program, "x"), volume.info.x);
	glUniform1i(glGetUniformLocation(computeShader.program, "y"), volume.info.y);
	glUniform1i(glGetUniformLocation(computeShader.program, "z"), volume.info.z);
	glUniform3f(glGetUniformLocation(computeShader.program, "d"), volume.info.d.x, volume.info.d.y, volume.info.d.z);
	glUniform3f(glGetUniformLocation(computeShader.program, "min"), volume.info.min.x, volume.info.min.y, volume.info.min.z);
	glUniform3f(glGetUniformLocation(computeShader.program, "max"), volume.info.max.x, volume.info.max.y, volume.info.max.z);


	glDispatchCompute(SCREEN_WIDTH, SCREEN_HEIGHT, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	cout << "DONE" << endl;

	glDeleteBuffers(2, &buffers[0]);
	
	glutMainLoop();

	delete texture;

	return 0;
}