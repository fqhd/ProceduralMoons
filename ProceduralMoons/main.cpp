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

int main() {
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIDTH, WIDTH, "Window", nullptr, nullptr);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwMakeContextCurrent(window);
	gladLoadGL();
	
	GLuint vertID;
	GLuint fragID;

	std::string vsCode = Utils::readFileToString("shaders/shader.vert");
	std::string fsCode = Utils::readFileToString("shaders/shader.frag");

	const char* vs_pointer = vsCode.c_str();
	const char* fs_pointer = fsCode.c_str();

	// Vertex Shader
	vertID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertID, 1, &vs_pointer, NULL);
	glCompileShader(vertID);
	Utils::checkCompileErrors(vertID, "VERTEX");
	
	// Fragment Shader
	fragID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragID, 1, &fs_pointer, NULL);
	glCompileShader(fragID);
	Utils::checkCompileErrors(fragID, "FRAGMENT");

	// shader Program
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertID);
	glAttachShader(programID, fragID);
	glLinkProgram(programID);
	glValidateProgram(programID);
	Utils::checkCompileErrors(programID, "PROGRAM");

	glUseProgram(programID);

	float vertexData[] = {
		-1, -1,
		-1, 1,
		1, 1,
		-1, -1,
		1, 1,
		1, -1
	};

	GLuint vaoID, vboID;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	uint8_t* imageData = new uint8_t[WIDTH * WIDTH * 3];

	glfwPollEvents();
	Benchmarker::Start("Test");
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glReadPixels(0, 0, WIDTH, WIDTH, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	Benchmarker::End("Test");
	glfwSwapBuffers(window);

	stbi_write_jpg("output.jpg", WIDTH, WIDTH, 3, imageData, 90);
	delete[] imageData;

	glfwDestroyWindow(window);
	glfwTerminate();
}