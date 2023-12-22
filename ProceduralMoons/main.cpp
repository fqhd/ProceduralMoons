#include <iostream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "glad.h"
#include <glm/glm.hpp>
#include "Benchmarker.h"
#include "Utils.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

const int WIDTH = 2048;

void readTexturePixels(GLuint textureID, int width, int height, unsigned char* pixels) {
	glBindTexture(GL_TEXTURE_2D, textureID);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
}

int main() {
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(1024, 1024, "Window", nullptr, nullptr);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwMakeContextCurrent(window);
	gladLoadGL();

	GLuint computeID;

	std::string csCode = Utils::readFileToString("shaders/shader.comp");

	const char* cs_pointer = csCode.c_str();

	computeID = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(computeID, 1, &cs_pointer, NULL);
	glCompileShader(computeID);
	Utils::checkCompileErrors(computeID, "COMPUTE");

	GLuint programID = glCreateProgram();
	glAttachShader(programID, computeID);
	glLinkProgram(programID);
	Utils::checkCompileErrors(programID, "PROGRAM");
	glUseProgram(programID);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WIDTH, WIDTH, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindImageTexture(0, textureID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

	uint8_t* imageData = new uint8_t[WIDTH * WIDTH * 4];

	Benchmarker::Start("Test");

	glDispatchCompute(WIDTH, WIDTH, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	readTexturePixels(textureID, WIDTH, WIDTH, imageData);

	Benchmarker::End("Test");

	stbi_write_jpg("output.jpg", WIDTH, WIDTH, 4, imageData, 90);

	glfwDestroyWindow(window);
	glfwTerminate();
}